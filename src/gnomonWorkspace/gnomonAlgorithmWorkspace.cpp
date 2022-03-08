#include "gnomonAlgorithmWorkspace.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonAbstractCommand.h>
#include <gnomonPipeline>
#include <gnomonVisualization>

#include <dtkCore>
#include <dtkImagingCore>
#include <dtkScript>

#include <vtkImageData.h>
#include <vtkRenderer.h>

// /////////////////////////////////////////////////////////////////////////////
// gnomonAlgorithmWorkspacePrivate
// /////////////////////////////////////////////////////////////////////////////


gnomonAlgorithmWorkspacePrivate::gnomonAlgorithmWorkspacePrivate(void)
{
    this->pipeline_manager = gnomonPipelineManager::instance();
}

gnomonAlgorithmWorkspacePrivate::~gnomonAlgorithmWorkspacePrivate(void)
{
}

bool gnomonAlgorithmWorkspacePrivate::setAlgorithm(const QString& algorithm)
{
    if (algorithm != this->algorithm) {
        this->command->setAlgorithmName(algorithm);
        this->algorithm = algorithm;
        return true;
    }
    return false;
}

void gnomonAlgorithmWorkspacePrivate::registerPipeline(void)
{
    if (this->command) {
        this->pipeline_manager->addAlgorithm(this->command);
    }
}

void gnomonAlgorithmWorkspacePrivate::updatePool(void)
{
    if(!this->pool)
        this->pool = new gnomonViewFormPool(this);

    foreach(gnomonViewForm *view, this->sources->views())
        this->pool->addView(view);

    foreach(gnomonViewForm *view, this->targets->views())
        this->pool->addView(view);
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonAlgorithmWorkspace
// /////////////////////////////////////////////////////////////////////////////

gnomonAlgorithmWorkspace::gnomonAlgorithmWorkspace(QObject *parent) : gnomonAbstractWorkspace(parent)
{
    d = new gnomonAlgorithmWorkspacePrivate;

    d->sources = new gnomonViewFormList(this);
    connect(d->sources, &gnomonViewFormList::viewAdded, [=] (gnomonViewForm *v) {
        v->setInputView(true);
    });

    d->targets = new gnomonViewFormList(this);
    connect(d->targets, &gnomonViewFormList::viewAdded, [=] (gnomonViewForm *v) {
        connect(v, &gnomonViewForm::exportedForm, [=] (gnomonAbstractDynamicForm *f) {
            d->pipeline_manager->addForm(f);
        });
    });

    connect(d->sources, &gnomonViewFormList::formsChanged, [=] ()
    {
        this->setInputs();
        emit parametersChanged();
    });

    connect(this, &gnomonAlgorithmWorkspace::parametersChanged, this, &gnomonAlgorithmWorkspace::saveState);
}

gnomonAlgorithmWorkspace::~gnomonAlgorithmWorkspace(void)
{
    delete d;
}

QString gnomonAlgorithmWorkspace::algoName(void) const
{
    return d->algorithm;
}

QStringList gnomonAlgorithmWorkspace::algorithms(void) const
{
    return d->keys;
}

void gnomonAlgorithmWorkspace::setAlgoName(const QString& algorithm)
{
    if (d->setAlgorithm(algorithm)) {
        emit algorithmChanged(algorithm);
        d->command->undo();
        this->setInputs();
        emit parametersChanged();
    }
}

int gnomonAlgorithmWorkspace::currentIndex(void) const {
    return d->currentIndex;
}

void gnomonAlgorithmWorkspace::setCurrentIndex(int i) {
    if ((d->currentIndex != i) & (i < d->keys.size())) {
        d->currentIndex = i;
        emit currentIndexChanged();
        this->setAlgoName(d->keys[i]);
    }
}

QJSValue gnomonAlgorithmWorkspace::parameters(void)
{
    return dtkCoreParameterCollection(d->command->parameters()).toJSValue(this);
}

gnomonViewFormList* gnomonAlgorithmWorkspace::sources(void) const
{
    return d->sources;
}

gnomonViewFormList* gnomonAlgorithmWorkspace::targets(void) const
{
    return d->targets;
}

void gnomonAlgorithmWorkspace::run(bool no_async)
{
    Q_ASSERT(d->command);

    emit started();

    this->setInputs();

    if(no_async){
        d->command->setNoAsync();
    }

    d->command->redo();

    this->viewOutputs();
}

void gnomonAlgorithmWorkspace::setInputs()
{
    // you need to overwrite this function if you don't have an exact mapping between
    // the number of views (sources) and the number of input types for your command,
    // and that all inputs can not be loaded from a single view.

    if (d->sources->views().size() == 1) {
        for(auto [name, input_type] : d->command->inputTypes()) {
            d->command->setInputForm(name, (*d->sources)[0]->form(input_type));
        }
    } else  if (d->command->inputs().size() == d->sources->views().size()) {
        int i=0;
        for(auto [name, input_type] : d->command->inputTypes()) {
            d->command->setInputForm(name, (*d->sources)[i]->form(input_type));
            ++i;
        }
    } else {
        dtkWarn() << Q_FUNC_INFO << "inputs size " <<d->command->inputs().size() << " but nb input views " << d->sources->views().size();
        return;
    }
}

void gnomonAlgorithmWorkspace::viewOutputs(void)
{
    Q_ASSERT(d->command);

    if((d->targets->views().size()>1) & (d->command->outputs().size() != d->targets->views().size())) {
        dtkWarn() << Q_FUNC_INFO << "outputs size " <<d->command->outputs().size() << " but nb output views " << d->targets->views().size();
        return;
    }

    bool empty_output = true;

    int i=0;
    gnomonAbstractDynamicForm* inputForm = nullptr;
    for(auto [name, output_type] : d->command->inputTypes()) {
        if (d->command->inputs()[name]) {
            inputForm = d->command->inputs()[name];
            break;
        }
    }

    for(auto [name, output_type] : d->command->outputTypes()) {
        if(d->command->outputs()[name]) {
            auto form =  d->command->outputs()[name];
            int form_count = gnomonFormManager::instance()->formCount(output_type);
            form->metadata()->set("name", output_type.remove("gnomon") + QString::number(form_count+1));
            form->metadata()->set("source", d->algorithm);
            (*d->targets)[i]->setForm(output_type, form);
            (*d->targets)[i]->render();
            empty_output = false;
        }
        if(d->targets->views().size()>1)
            ++i;
    }

    if (!empty_output) {
        d->registerPipeline();
        this->target()->tryLinking();
    }
}

QJsonObject gnomonAlgorithmWorkspace::serialize(void) {
    QJsonObject state;
    state.insert("algoName", algoName());
    state.insert("currentIndex", currentIndex());

    QVariantMap parameters_json;
    dtkCoreParameters dtkParameters = d->command->parameters();
    for(const auto& param_name : dtkParameters.keys()){
        QVariant param_value = dtkParameters[param_name]->variant();
        qDebug()<<Q_FUNC_INFO<<param_name<<param_value;
        parameters_json.insert(param_name, param_value);
    }
    state.insert("parameters_json", QJsonObject::fromVariantMap(parameters_json));
    return state;
}

void gnomonAlgorithmWorkspace::unSerialize(QJsonObject & state) {
    setCurrentIndex(state["currentIndex"].toInt());
    setAlgoName(state["algoName"].toString());

    QJsonObject parameters_json = state["parameters"].toObject();
    for(const auto& param_name: parameters_json.keys()) {
        QVariant param = parameters_json[param_name].toVariant();
        d->command->setParameter(param_name, param);
    }
    emit parametersChanged();
}

void gnomonAlgorithmWorkspace::saveState(void) {
    d->savedState = serialize();
}

void gnomonAlgorithmWorkspace::restoreState(void) {
    QString previousAlgo = algoName();
    unSerialize(d->savedState);
}

void gnomonAlgorithmWorkspace::addInputView(const QVector<QString>& accepted_forms) {
    if(accepted_forms.empty()) {
        QVector<QString> default_forms;
        for(auto [name, input_type] : d->command->inputTypes()) {
            default_forms.push_back(input_type);
        }
        d->sources->addView(default_forms);
    } else {
        d->sources->addView(accepted_forms);
    }
}

void gnomonAlgorithmWorkspace::addOutputView(const QVector<QString> &accepted_forms) {
    if(accepted_forms.empty()) {
        QVector<QString> default_forms;
        for(auto [name, input_type] : d->command->outputTypes()) {
            default_forms.push_back(input_type);
        }
        d->targets->addView(default_forms);
    } else {
        d->targets->addView(accepted_forms);
    }
}

//
// gnomonAlgorithmWorkspace.cpp ends here
