#include "gnomonWorkspaceLSystemModel.h"

#include <gnomonCore/gnomonModel/gnomonAbstractLStringEvolutionModel>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringEvolutionModelCommand>
#include <gnomonCore/gnomonPythonPluginLoader>

#include <gnomonPipeline/gnomonPipelineManager.h>
#include <gnomonProject/gnomonProjectManager.h>
#include <gnomonProject/gnomonProject.h>

#include <gnomonVisualization/gnomonView/gnomonVtkView>
#include <gnomonVisualization/gnomonView/gnomonQmlView>
#include "gnomonCommand/gnomonLString/gnomonLStringEvolutionModelCommand.h"
#include "gnomonForm/gnomonLString/gnomonLString.h"
#include "gnomonVisualizations/gnomonLString/gnomonAbstractLStringVtkVisualization"
#include <dtkScript>

QString vonKochLSystem(void)
{
    QString l_sys;
    l_sys += "Axiom: -(90)_(0.1)F(50.)\n";
    l_sys += "derivation length: 5\n";
    l_sys += "production:\n";
    l_sys += "F(x) : produce F(x/3.)+(60)F(x/3.)-(120)F(x/3.)+(60)F(x/3.)\n";
    l_sys += "endlsystem\n";
    l_sys += "###### INITIALISATION ######\n";
    l_sys += "\n";
    l_sys += "def __initialiseContext__(context):\n";
    l_sys += "    from openalea.plantgl.all import Material,Color3\n";
    l_sys += "    context.options.setSelection(\"Selection Required\",0)\n";
    l_sys += "    context.options.setSelection(\"Module declaration\",0)\n";
    return l_sys;
}

QString filePathFromUrl(const QString& file_url)
{
    QString file_path;
    const QUrl url(file_url);
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    if (url.isLocalFile()) {
        file_path = QDir::toNativeSeparators(url.toLocalFile());
    } else {
        file_path = file_url;
    }
    return file_path;
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceLSystemModelPrivate
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceLSystemModelPrivate
{
public:
    gnomonWorkspaceLSystemModelPrivate(void);
    ~gnomonWorkspaceLSystemModelPrivate(void);

public:
    bool setModel(const QString& model);

public:
    QString text;
    QString message;
    int animation_step = 1;

    int derivations = 0;
    std::shared_ptr<gnomonLStringSeries> current_lString;

public:
    QString workspace;
    QStringList keys;
    QString model;
    QString file;
    int currentIndex = 0;

    QDir* tmpDir = nullptr;
    QFile* model_file = nullptr;
    QFuture<int> redo_future;

    //QMutex mutex;
    //QWaitCondition synchro;

public:
    gnomonLStringEvolutionModelCommand *command = nullptr;

public:
    gnomonVtkView *view = nullptr;
    gnomonQmlView *text_view = nullptr;
    gnomonProjectManager *project_manager = nullptr;
    QJsonObject state;
};

gnomonWorkspaceLSystemModelPrivate::gnomonWorkspaceLSystemModelPrivate(void)
{

}

gnomonWorkspaceLSystemModelPrivate::~gnomonWorkspaceLSystemModelPrivate(void)
{

}


// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceLSystemModel
// /////////////////////////////////////////////////////////////////////////////


gnomonWorkspaceLSystemModel::gnomonWorkspaceLSystemModel(QObject *parent) : gnomonAbstractWorkspace(parent)
{
    d = new gnomonWorkspaceLSystemModelPrivate;
    d->workspace = "LStringEvolutionModel";
    d->command = new gnomonLStringEvolutionModelCommand;
    emit modelsLoaded();
    d->keys = gnomonCore::lStringEvolutionModel::pluginFactory().keys();
    d->model = d->command->modelName();
    d->project_manager = gnomonProjectManager::instance();
    auto temp_dir = d->project_manager->project()->projectDir() + "/.gnomon";
    d->tmpDir =  new QDir(temp_dir);
    
    int stat;
    QString temp_working_directory = "";
    temp_working_directory += "import sys \n";
    temp_working_directory += "cwdir = ";
    temp_working_directory += "'" + d->tmpDir->path() + "'" + "\n";
    temp_working_directory += "if not sys.path.__contains__(f'{cwdir}'): \n";
    temp_working_directory += " sys.path.append(f'{cwdir}')\n";

    dtkScriptInterpreterPython::instance()->interpret(temp_working_directory, &stat);

    d->view = new gnomonVtkView(this);
    d->view->setNodePortNames({});
    d->view->setAcceptForm("gnomonLString", true);

    d->text_view = new gnomonQmlView(this);
    d->text_view->setAcceptForm("gnomonLString", true);

    connect(d->view, &gnomonVtkView::formAdded, [=](const QString &name) {
        const QString plugin_name = "lStringVisualizationVtkTurtle";
        if (name == "gnomonLString") {
            if (d->view->formVisuName(name) == plugin_name) {
                // d->view->setFormVisuName(name, plugin_name);
                d->view->setFormVisuParameter(name, "interpretation_lsystem", d->model_file->fileName());
            }
            auto visu_params = d->view->formVisuParameters(name);
            QJSValueIterator it(visu_params);
            while (it.hasNext()) {
                it.next();
            }
        }
    });

    connect(d->view, &gnomonVtkView::formVisualizationChanged, [=](const QString &name) {
        const QString plugin_name = "lStringVisualizationVtkTurtle";
        if (name == "gnomonLString") {
            if (d->view->formVisuName(name) == plugin_name) {
                d->view->setFormVisuParameter(name, "interpretation_lsystem", d->model_file->fileName());
            }
        }
    });

    connect(d->view, &gnomonVtkView::exportedForm, [=] (std::shared_ptr<gnomonAbstractDynamicForm> f) {
        gnomonPipelineManager::instance()->addForm(f);
        this->m_can_be_destroyed = false;
        emit canBeDestroyedChanged(false);
    });
    connect(d->text_view, &gnomonQmlView::exportedForm, [=] (std::shared_ptr<gnomonAbstractDynamicForm> f) {
        gnomonPipelineManager::instance()->addForm(f);
        this->m_can_be_destroyed = false;
        emit canBeDestroyedChanged(false);
    });

    connect(d->command, &gnomonAbstractEvolutionModelCommand::modelMessage, [=](QString msg) {
            d->message = msg;
            this->messageChanged();
    });

    this->setDefaultLSystem();
}

gnomonWorkspaceLSystemModel::~gnomonWorkspaceLSystemModel(void)
{
    gnomonLStringEvolutionModelCommand *command = (gnomonLStringEvolutionModelCommand *) d->command;
    if (command) {
        delete command;
        command = nullptr;
    }

    if(d->model_file) {
        d->model_file->remove();
        delete d->model_file;
        d->model_file = nullptr;
    }


    delete d;
}

QString gnomonWorkspaceLSystemModel::text(void)
{
    return d->text;
}

QString gnomonWorkspaceLSystemModel::message(void) const
{
    return d->message;
}

void gnomonWorkspaceLSystemModel::setText(const QString& text)
{
    if (text != d->text) {
        d->text = text;

        if(!d->model_file || d->model_file->fileName() != d->tmpDir->filePath(d->file)) {
            d->model_file = new QFile(d->tmpDir->filePath(d->file));
        }

        if (d->model_file->open(QIODevice::WriteOnly)) {
            QTextStream model_stream(d->model_file);
            model_stream<<d->text;
            model_stream.flush();

            d->model_file->close();
            d->command->setLSystem(d->model_file->fileName());
            this->reset();
        } else {
            qWarning() << "cannot open temp file for writing" << d->model_file;
        }
        emit derivationLengthChanged(d->command->derivationLength());
        emit textChanged(d->text);
        emit parametersChanged();
    }
}

int gnomonWorkspaceLSystemModel::derivationLength(void)
{
    return d->command->derivationLength();
}

void gnomonWorkspaceLSystemModel::setDerivationLength(int l)
{
    d->command->setDerivationLength(l);
    emit derivationLengthChanged(l);
}

int gnomonWorkspaceLSystemModel::animationStep(void)
{
    return d->animation_step;
}

void gnomonWorkspaceLSystemModel::setAnimationStep(int s)
{
    if (s != d->animation_step) {
        d->animation_step = s;
        emit animationStepChanged(d->animation_step);
    }
}

void gnomonWorkspaceLSystemModel::setAnimationTime(const QString& time)
{
    bool ok;
    double t_double = time.toDouble(&ok);
    if(ok) {
        d->command->setAnimationTime(t_double);
    } else {
        qWarning() << "SetAnimationTime: Cannot convert " << time << " to double";
    }
}

void gnomonWorkspaceLSystemModel::read(const QString& file_url)
{
    QString file_name = file_url.split(QRegularExpression("/")).last();
    QString file_path = filePathFromUrl(file_url);

    QFile f(file_path);
    QFileInfo finfo(file_path);
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream in(&f);

        if(d->model_file) {
            delete d->model_file;
        }

        d->model_file = new QFile(d->tmpDir->filePath(file_name));
        this->setFileName(file_name);
        this->setText(in.readAll());
        this->reset();
    } else {
        dtkWarn()<<"Could not open file"<<file_path;
    }
}

void gnomonWorkspaceLSystemModel::save(const QString& file_url) const
{
    QString file_path = filePathFromUrl(file_url);

    QFile f(file_path);
    if(f.open(QIODevice::WriteOnly| QIODevice::Text)) {
        QTextStream out(&f);
        out<<d->text;
        f.close();
    } else {
        dtkWarn()<<"Could not save to file"<<file_path;
    }
}

void gnomonWorkspaceLSystemModel::setDefaultLSystem(void)
{
    this->setFileName("vonKoch.lpy");
    this->setText(vonKochLSystem());
}

void gnomonWorkspaceLSystemModel::animate()
{
    Q_ASSERT(d->command);

    emit started();
    this->setInitialState();
    d->command->undo();

    d->derivations = 0;
    d->command->simulationType = SimulationType::animate;
    d->command->setAnimationStep(d->animation_step);
    connect(d->command, &gnomonLStringEvolutionModelCommand::stepFinished, [=] (int s){
        d->derivations = s;
        this->viewNewStep();
    });
    connect(d->command, &gnomonLStringEvolutionModelCommand::finished, [=](){
        disconnect(d->command, &gnomonLStringEvolutionModelCommand::finished, nullptr, nullptr);
        disconnect(d->command, &gnomonLStringEvolutionModelCommand::stepFinished, nullptr, nullptr);
        emit finished();
    });
    d->redo_future = d->command->redo(nullptr, nullptr); //&d->mutex, &d->synchro);
}

void gnomonWorkspaceLSystemModel::run()
{
    Q_ASSERT(d->command);

    emit started();
    this->setInitialState();
    d->command->undo();
    d->command->simulationType = SimulationType::run;
    connect(d->command, &gnomonLStringEvolutionModelCommand::finished, [=](){
        disconnect(d->command, &gnomonLStringEvolutionModelCommand::finished, nullptr, nullptr);
        d->derivations = derivationLength();
        this->viewState();
        emit finished();
    });

    d->command->redo();
}

void gnomonWorkspaceLSystemModel::step()
{
    Q_ASSERT(d->command);

    emit started();
    auto lString = d->command->lString();
    if (!lString || lString->times().size() == 0) {
        this->setInitialState();
        d->command->undo();
        d->derivations = 0;
    }
    d->command->simulationType = SimulationType::step;

    connect(d->command, &gnomonLStringEvolutionModelCommand::finished, [=](){
        disconnect(d->command, &gnomonLStringEvolutionModelCommand::finished, nullptr, nullptr);
        d->derivations += 1;
        this->viewState();
        emit finished();
    });

    d->command->redo();
}

void gnomonWorkspaceLSystemModel::reset()
{
    Q_ASSERT(d->command);

    emit started();
    this->setInitialState();
    d->command->undo();
    emit derivationLengthChanged(derivationLength());
    d->derivations = 0;
    this->viewState();
    emit finished();
}

void gnomonWorkspaceLSystemModel::stop()
{
    Q_ASSERT(d->command);
    d->redo_future.cancel();
    qDebug() << "cancel launched , only works on animate for now";
}

QUrl gnomonWorkspaceLSystemModel::defaultReadPath(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    return settings.value("lsystem/path").toString();
}

void gnomonWorkspaceLSystemModel::setInitialState()
{
    // TODO: drop axiom into a different view?
    // d->command->setAxiom(d->view->lString());
    d->command->setAxiom(nullptr);
}

void gnomonWorkspaceLSystemModel::viewState()
{
    // TODO: pass lsystem to visu plugin
    auto lString = d->command->lString();
    if (lString) {
        d->view->setForm("gnomonLString", lString); //TODO only update, only do it if it's different ..
        if (lString->times().size() != 0) {
            d->view->setCurrentTime(lString->times().last());
        }
        d->view->render();

        d->text_view->setForm("gnomonLString", lString);

        gnomonPipelineManager::instance()->addEvolutionModel(d->command);
    }
}

void gnomonWorkspaceLSystemModel::viewNewStep()
{
    // get the current lstring from view
    auto lString = d->view->lString();
    if(!lString || d->derivations == d->animation_step) {
        lString = d->command->lString();
        if(lString && lString->times().length() > 0) {
            d->view->setForm("gnomonLString", lString);
            d->text_view->setForm("gnomonLString", lString);
        } else {
            return;
        }
    }

    if (lString->times().size() != 0) {
        d->view->formAdded("gnomonLString");
        d->view->setCurrentTime(d->derivations);
    }
    //gnomonPipelineManager::instance()->addEvolutionModel(d->command);
}


void gnomonWorkspaceLSystemModel::export_outputs(void)
{
    d->view->transmit();
}

QString gnomonWorkspaceLSystemModel::modelName(void) const
{
    return d->model;
}

QString gnomonWorkspaceLSystemModel::fileName(void) const
{
    return d->file;
}

QStringList gnomonWorkspaceLSystemModel::models(void) const
{
    return d->keys;
}

void gnomonWorkspaceLSystemModel::setModelName(const QString &model)
{
    if (model != d->model) {
        d->command->setModelName(model);
        d->model = model;
        emit modelChanged(model);
        d->command->undo();
        this->setInitialState();
        emit parametersChanged();
    }
}

void gnomonWorkspaceLSystemModel::setFileName(const QString &filename)
{
    if(filename != d->file) {
        d->file = filename;
        emit fileChanged(filename);
    }
}

int gnomonWorkspaceLSystemModel::currentIndex(void) const
{
    return d->currentIndex;
}

void gnomonWorkspaceLSystemModel::setCurrentIndex(int i)
{
    if ((d->currentIndex != i) & (i < d->keys.size())) {
        d->currentIndex = i;
        emit currentIndexChanged();
        this->setModelName(d->keys[i]);
    }
}

gnomonVtkView *gnomonWorkspaceLSystemModel::view(void) const
{
    return d->view;
}

gnomonQmlView *gnomonWorkspaceLSystemModel::textView(void) const
{
    return d->text_view;
}

QJSValue gnomonWorkspaceLSystemModel::parameters(void)
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

QJsonObject gnomonWorkspaceLSystemModel::serialize() {
    QJsonObject state;

    state.insert("text", d->text);
    state.insert("filename", d->file);
    state.insert("currentIndex", d->currentIndex);
    state.insert("model", d->model);
    state.insert("derivations", d->derivations);
    state.insert("derivation_length", derivationLength());
    state.insert("animation_step", d->animation_step);

    QVariantMap parameters_json;
    dtkCoreParameters dtkParameters = d->command->parameters();
    for(const auto& param_name : dtkParameters.keys()){
        auto param_value = dtkParameters[param_name]->toVariantHash();
        parameters_json.insert(param_name, QJsonObject::fromVariantHash(param_value));
    }
    state.insert("parameters", QJsonObject::fromVariantMap(parameters_json));

    return state;
}

void gnomonWorkspaceLSystemModel::unSerialize(const QJsonObject &state) {
    QJsonObject parameters_json = state["parameters"].toObject();

    setFileName(state["filename"].toString());
    setText(state["text"].toString());
    setDerivationLength(state["derivation_length"].toInt());
    d->derivations = state["derivations"].toInt();
    setAnimationStep(state["animation_step"].toInt());
    setModelName(state["model"].toString());

    for(const auto& param_name: parameters_json.keys()) {
        auto param = parameters_json[param_name].toObject().toVariantHash();
        d->command->setParameter(param_name, dtkCoreParameter::create(param)->variant());
    }
    d->view->restoreState();
}

void gnomonWorkspaceLSystemModel::saveState() {
    d->state = serialize();
}

void gnomonWorkspaceLSystemModel::restoreState() {
    unSerialize(d->state);
}
