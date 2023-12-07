#include "gnomonAlgorithmWorkspace.h"
#include "gnomonAlgorithmWorkspace_p.h"
#include "gnomonProject"

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand.h>

#include <gnomonPipeline/gnomonPipelineManager.h>

#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>
#include <gnomonVisualization/gnomonView/gnomonVtkViewPool.h>

// /////////////////////////////////////////////////////////////////////////////
// gnomonAlgorithmWorkspacePrivate
// /////////////////////////////////////////////////////////////////////////////


gnomonAlgorithmWorkspacePrivate::gnomonAlgorithmWorkspacePrivate(void)
{
    this->pipeline_manager = gnomonPipelineManager::instance();
}

gnomonAlgorithmWorkspacePrivate::~gnomonAlgorithmWorkspacePrivate(void)
{
    if(this->pool)
        delete pool;

    if(this->sources)
        delete this->sources;

    if(this->targets)
        delete this->targets;

    if (this->figure)
        delete this->figure;

    if (this->text_view)
        delete this->text_view;
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
        this->pool = new gnomonVtkViewPool(this);

    foreach(gnomonVtkView *view, this->sources->views())
        this->pool->addView(view);

    foreach(gnomonVtkView *view, this->targets->views())
        this->pool->addView(view);
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonAlgorithmWorkspace
// /////////////////////////////////////////////////////////////////////////////

gnomonAlgorithmWorkspace::gnomonAlgorithmWorkspace(QObject *parent) : gnomonAbstractWorkspace(parent)
{
    d = new gnomonAlgorithmWorkspacePrivate;

    d->sources = new gnomonVtkViewList(this);
    connect(d->sources, &gnomonVtkViewList::viewAdded, [=] (gnomonVtkView *v) {
        v->setInputView(true);
    });

    d->targets = new gnomonVtkViewList(this);
    connect(d->targets, &gnomonVtkViewList::viewAdded, [=] (gnomonVtkView *v) {
        connect(v, &gnomonVtkView::exportedForm, [=] (std::shared_ptr<gnomonAbstractDynamicForm> f) {
            d->registerPipeline();
            d->pipeline_manager->addForm(f->uuid());
            this->m_can_be_destroyed = false;
            emit canBeDestroyedChanged(false);
        });
    });

    connect(d->sources, &gnomonVtkViewList::formsChanged, [=] ()
    {
        this->setInputs();
        emit parametersChanged();
    });

    connect(this, &gnomonAlgorithmWorkspace::parametersChanged, this, &gnomonAlgorithmWorkspace::saveState);
    connect(this, &gnomonAlgorithmWorkspace::parametersChanged, [=] () {
        emit stateChanged();
    });

    d->timer.setInterval(100);
    connect(&d->timer, &QTimer::timeout, [=]() {
        //qDebug() << "============= PROGRESS : " << d->command->progress();
        emit progressChanged(d->command->progress());
    });
    connect(this, &gnomonAbstractWorkspace::started, [=]() {
       d->timer.start();
    });
    connect(this, &gnomonAbstractWorkspace::finished, [=]() {
        d->timer.stop();
    });
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

QVariantList gnomonAlgorithmWorkspace::algorithmsData(void) const
{
    return d->algorithmsData;
}

void gnomonAlgorithmWorkspace::setAlgoName(const QString& algorithm)
{
    if (d->setAlgorithm(algorithm)) {
        emit algorithmChanged(algorithm);
        d->command->undo();
        this->setInputs();
        for(auto & param:d->command->parameters()) {
            param->connect([=] {
               emit parametersChanged();
            });
        }
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

void gnomonAlgorithmWorkspace::export_outputs(void) {
    for(const auto &output_view: d->targets->views()) {
        output_view->transmit();
    }
}

QJSValue gnomonAlgorithmWorkspace::parameters(void)
{
    QJSValue parameters = dtkCoreParameterCollection(d->command->parameters()).toJSValue(this);
    QMap<QString, QString> parameter_groups = d->command->parameterGroups();

    QJSValueIterator it(parameters);
    while (it.hasNext()) {
        it.next();
        QString group = parameter_groups.contains(it.name()) ? parameter_groups[it.name()] : "";
        it.value().setProperty("group", group != "" ? group : nullptr);
    }
    return parameters;
}

gnomonVtkViewList* gnomonAlgorithmWorkspace::sources(void) const
{
    return d->sources;
}

gnomonVtkViewList* gnomonAlgorithmWorkspace::targets(void) const
{
    return d->targets;
}

gnomonMplView* gnomonAlgorithmWorkspace::figure(void) const
{
    return d->figure;
}

gnomonQmlView* gnomonAlgorithmWorkspace::textView(void) const
{
    return d->text_view;
}

void gnomonAlgorithmWorkspace::run(bool no_async)
{
    Q_ASSERT(d->command);

    disconnect(d->connect_finished);
    if(!no_async) {
        d->connect_finished = connect(d->command, &gnomonAbstractCommand::finished, [this]() {
            this->viewOutputs();
            emit finished();
        });
    }

    emit started();

    // this->setInputs();

    if(no_async){
        d->command->setNoAsync();
    }

    d->command->redo();

    if(no_async){
        this->viewOutputs();
    }
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

    for(auto [name, output_type] : d->command->outputTypes()) {
        if(d->command->outputs()[name] && (*d->targets)[i]->acceptedForms().contains(output_type)) {
            auto form =  d->command->outputs()[name];
            GNOMON_SESSION->registerForm(form);
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
        if(!this->target()->synced()) {
            this->target()->tryLinking();
        }
    }
}

QJsonObject gnomonAlgorithmWorkspace::serialize(void) {
    QJsonObject state;
    state.insert("algoName", algoName());
    state.insert("currentIndex", currentIndex());

    QVariantMap parameters_json;
    dtkCoreParameters dtkParameters = d->command->parameters();
    for(const auto& param_name : dtkParameters.keys()){
        auto param_value = dtkParameters[param_name]->toVariantHash();
        parameters_json.insert(param_name, QJsonObject::fromVariantHash(param_value));
    }
    state.insert("parameters", QJsonObject::fromVariantMap(parameters_json));
    return state;
}

void gnomonAlgorithmWorkspace::unSerialize(const QJsonObject & state) {
    setCurrentIndex(state["currentIndex"].toInt());
    setAlgoName(state["algoName"].toString());

    QJsonObject parameters_json = state["parameters"].toObject();
    for(const auto& param_name: parameters_json.keys()) {
        auto param = parameters_json[param_name].toObject().toVariantHash();
        d->command->setParameter(param_name, dtkCoreParameter::create(param)->variant());
    }

    for (auto view : d->sources->views()) {
        view->restoreState();
    }
    for (auto view : d->targets->views()) {
        view->restoreState();
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

void gnomonAlgorithmWorkspace::addInputView(const QVector<QString>& accepted_forms, QStringList nodePortNames) {
    if(nodePortNames.isEmpty()) {
        nodePortNames = d->command->inputs().keys();
    }

    if(accepted_forms.empty()) {
        QVector<QString> default_forms;
        for(auto [name, input_type] : d->command->inputTypes()) {
            default_forms.push_back(input_type);
        }
        d->sources->addView(default_forms, nodePortNames);
    } else {
        d->sources->addView(accepted_forms, nodePortNames);
    }
}

void gnomonAlgorithmWorkspace::addOutputView(const QVector<QString> &accepted_forms, QStringList nodePortNames) {
    if(nodePortNames.isEmpty()) {
        nodePortNames = d->command->outputs().keys();
    }

    if(accepted_forms.empty()) {
        QVector<QString> default_forms;
        for(auto [name, input_type] : d->command->outputTypes()) {
            default_forms.push_back(input_type);
        }
        d->targets->addView(default_forms, nodePortNames);
    } else {
        d->targets->addView(accepted_forms, nodePortNames);
    }
}

int gnomonAlgorithmWorkspace::progress(void) {
    return d->command->progress();
}

void gnomonAlgorithmWorkspace::pause(void) {
    d->command->pause();
}

void gnomonAlgorithmWorkspace::resume(void) {
    d->command->resume();
}

void gnomonAlgorithmWorkspace::stop(void) {
    d->command->stop();
}

//
// gnomonAlgorithmWorkspace.cpp ends here
