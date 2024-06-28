#include "gnomonWorkspaceLSystemModel.h"

#include <gnomonCore/gnomonModel/gnomonAbstractLStringEvolutionModel>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringEvolutionModelCommand>
#include <gnomonCore/gnomonPythonPluginLoader>

#include <gnomonPipeline/gnomonPipelineManager.h>
#include "gnomonProject"

#include <gnomonVisualization/gnomonView/gnomonVtkView>
#include <gnomonVisualization/gnomonView/gnomonQmlView>
#include "gnomonCommand/gnomonLString/gnomonLStringEvolutionModelCommand.h"
#include "gnomonForm/gnomonLString/gnomonLString.h"
#include "gnomonVisualizations/gnomonLString/gnomonAbstractLStringVtkVisualization"
#include <dtkScript>
#include "gnomonVisualization/gnomonCoreParameterColorTableObject.h"

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

    QMap<QString, QString> open_files;

    QDir* lpy_dir = nullptr;
    QFile* model_file = nullptr;
    QFuture<int> redo_future;

    //QMutex mutex;
    //QWaitCondition synchro;

public:
    gnomonLStringEvolutionModelCommand *command = nullptr;

public:
    gnomonVtkView *view = nullptr;
    gnomonQmlView *text_view = nullptr;
    QJsonObject state;
    QStringList missing_textures;

    QMetaObject::Connection editor_connect;
    QTimer readFileTimer; // to keep d->text up to date with what's on the disk
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
    auto lpy_dir_path = GNOMON_PROJECT->projectDir() + "/.gnomon/lpy";
    d->lpy_dir =  new QDir(lpy_dir_path);
    d->lpy_dir->mkpath(lpy_dir_path);
    int stat;
    QString working_directory = "";
    working_directory += "import sys \n";
    working_directory += "cwdir = ";
    working_directory += "'" + GNOMON_PROJECT->projectDir() + "'" + "\n";
    working_directory += "if not sys.path.__contains__(f'{cwdir}'): \n";
    working_directory += " sys.path.append(f'{cwdir}')\n";

    dtkScriptInterpreterPython::instance()->interpret(working_directory, &stat);

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

            d->missing_textures.clear();
            QObject* parameters_object = qvariant_cast<QObject*>(visu_params.toVariant().toMap()["color_table"]);
            gnomonCoreParameterColorTableObject* params = dynamic_cast<gnomonCoreParameterColorTableObject*>(parameters_object);
            if(params) {
                for(const auto i: params->parameter()->colorIndices()) {
                    if(params->colorTable().isTexture(i))
                        if(GNOMON_PROJECT->findFile(params->colorTable().textureFile(i).split("/").last()).isEmpty())
                            d->missing_textures << params->colorTable().textureFile(i).split("/").last();
                }
                emit missingTexturesChanged();
            }
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
        gnomonPipelineManager::instance()->addForm(f->uuid());
        this->m_can_be_destroyed = false;
        emit canBeDestroyedChanged(false);
    });
    connect(d->text_view, &gnomonQmlView::exportedForm, [=] (std::shared_ptr<gnomonAbstractDynamicForm> f) {
        gnomonPipelineManager::instance()->addForm(f->uuid());
        this->m_can_be_destroyed = false;
        emit canBeDestroyedChanged(false);
    });

    connect(d->command, &gnomonAbstractEvolutionModelCommand::modelMessage, [=](QString msg) {
            d->message = msg;
            this->messageChanged();
    });

    connect(this, &gnomonWorkspaceLSystemModel::fileChanged,
            this, &gnomonWorkspaceLSystemModel::readOnlyChanged);

    d->readFileTimer.setInterval(1000);
    connect(&d->readFileTimer, &QTimer::timeout, this, &gnomonWorkspaceLSystemModel::updateFromCurrentFile);
    d->readFileTimer.start();

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
        delete d->model_file;
        d->model_file = nullptr;
    }

    // TODO: clear read-only files on exit?
    // for (auto lpy_file : d->lpy_dir->entryList(QStringList() << "*.lpy", QDir::Files)) {
    //     QFile(d->lpy_dir->filePath(lpy_file)).remove();
    // }

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

        if(!d->model_file) {
            d->model_file = new QFile(d->lpy_dir->filePath(d->file));
            d->open_files[d->file] = d->model_file->fileName();
        }

        if (d->model_file->open(QIODevice::WriteOnly)) {
            QTextStream model_stream(d->model_file);
            model_stream<<d->text;
            model_stream.flush();

            d->model_file->close();
            d->command->setLSystem(d->model_file->fileName());
            // TODO: doesn't that force recomputing / rendering at every character change?
            // this->reset();
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

void gnomonWorkspaceLSystemModel::updateFromCurrentFile() {
    QFile file(d->model_file->fileName());
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString new_text(in.readAll());
        file.close();
        this->setText(new_text);
    } else {
        dtkWarn() << "Could not open file" << d->model_file;
    }
}

// TODO: to factorize in a code editor workspace class
void gnomonWorkspaceLSystemModel::read(const QString& file_url, bool read_only, bool restoring)
{
    QString file_name = file_url.split(QRegularExpression("/")).last();
    QString relative_path = filePathFromUrl(file_url);

    QString absolute_path;
    if (!QFile::exists(relative_path)) {
        qDebug() << Q_FUNC_INFO << "file " << relative_path << "doesn't exist";
    } else {
        absolute_path = GNOMON_PROJECT->findFile(relative_path);
    }

    // FIXME: this condition should never happen (not read only and not in project)
    /* bool to_copy = (!read_only) && (absolute_path=="");
    if(to_copy) {
        QString file_name = relative_path.split(QRegularExpression("/")).last();
        QString project_file_path = GNOMON_PROJECT->projectDir() + "/" + file_name;
        if(!QFile::copy(relative_path, project_file_path)) {
            dtkWarn()<<"Failed to copy file "<< relative_path << "to Project";
            return;
        }
        relative_path = GNOMON_PROJECT->relativePath(project_file_path);
    } */

    if (!read_only) {
        d->open_files[file_name] = relative_path;
    } else {
        // Ensure read-only file is not modified by Python plugin by copying it in lpy_dir
        d->open_files[file_name] = d->lpy_dir->filePath(file_name);
        QFile::copy(relative_path, d->open_files[file_name]);
        // d->open_files[file_name] = relative_path;
    }

    this->setFileName(file_name);
    this->updateFromCurrentFile();

    if (!restoring) {
        this->reset();
    }
    if(!read_only)
        this->backup();
    emit stateChanged();
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

void gnomonWorkspaceLSystemModel::close(const QString& file_name)
{
    if (d->open_files.contains(file_name)) {
        d->open_files.remove(file_name);
        emit stateChanged();
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"The file"<<file_name<<"was not open";
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
        GNOMON_SESSION->trackForm(lString);
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

        if(d->model_file) {
            delete d->model_file;
        }
        if (d->open_files.contains(d->file)) {
            d->model_file = new QFile(d->open_files[d->file]);
        } else {
            d->model_file = nullptr;
        }

        // TODO: shouldn't we reset to use the axiom of the current model?
        // if(d->model_file) {
        //     d->command->setLSystem(d->model_file->fileName());
        //     this->reset();
        // }

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

QStringList gnomonWorkspaceLSystemModel::missingTextures(void) const
{
    return d->missing_textures;
}

void gnomonWorkspaceLSystemModel::copyTextureFiles(const QStringList& files)
{
    for(const auto& file : files) {
        auto new_file = GNOMON_PROJECT->projectDir() + "/" + file.split("/").last();
        QFile::copy(file, new_file);
    }
}

void gnomonWorkspaceLSystemModel::importFile(const QString& file_name, const QString& path)
{
    auto project_file = path + "/" + file_name;
    if (QFile::copy(d->open_files[file_name], project_file)) {
        QFile::remove(d->open_files[file_name]);
        this->backup();
        delete(d->model_file);
        d->open_files[file_name] = project_file;
        d->model_file = new QFile(project_file);
        emit fileChanged(file_name);
    }
}

QJsonObject gnomonWorkspaceLSystemModel::serialize() {
    QJsonObject state = gnomonAbstractWorkspace::serialize();

    QJsonObject open_file_json;
    for (const auto& file_name : d->open_files.keys()) {
        open_file_json.insert(file_name, d->open_files[file_name]);
    }
    state.insert("open_files", open_file_json);

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

    if (d->view) {
        state.insert("view", d->view->serialize());
    }
    if (d->text_view) {
        state.insert("text_view", d->text_view->serialize());
    }

    return state;
}

void gnomonWorkspaceLSystemModel::deserialize(const QJsonObject &state) {
    gnomonAbstractWorkspace::deserialize(state);
    disconnect(d->editor_connect);

    QJsonObject open_file_json = state["open_files"].toObject();
    for (auto file_name: open_file_json.keys()) {
        QString file_path = open_file_json[file_name].toString();
        d->open_files[file_name] = file_path;
    }

    d->editor_connect = connect(this, &gnomonWorkspaceLSystemModel::codeEditorReady, [=] () {
        for (auto file_name: d->open_files.keys()) {
            emit requestOpenFile(d->open_files[file_name]);
        }
    });

    QJsonObject parameters_json = state["parameters"].toObject();

    // TODO: to remove if code is restored from file / backup ?
    // setFileName(state["filename"].toString());
    // setText(state["text"].toString());
    setDerivationLength(state["derivation_length"].toInt());
    d->derivations = state["derivations"].toInt();
    setAnimationStep(state["animation_step"].toInt());
    setModelName(state["model"].toString());

    for(const auto& param_name: parameters_json.keys()) {
        auto param = parameters_json[param_name].toObject().toVariantHash();
        d->command->setParameter(param_name, dtkCoreParameter::create(param)->variant());
    }

    if (state.contains("view")) {
        d->view->deserialize(state.value("view").toObject());
    }
    if (state.contains("text_view")) {
        d->text_view->deserialize(state.value("text_view").toObject());
    }
}

void gnomonWorkspaceLSystemModel::saveState() {
    d->state = serialize();
}

void gnomonWorkspaceLSystemModel::restoreState() {
    deserialize(d->state);
}

bool gnomonWorkspaceLSystemModel::backup(void)
{
    QString file_name = d->file;
    bool ok = false;
    if (d->open_files.contains(file_name)) {
        QString file_path = d->open_files[file_name];
        ok = GNOMON_PROJECT->backupFile(file_path, d->text);
    }
    return ok;
}

void gnomonWorkspaceLSystemModel::restore()
{
    QStringList lpy_files = GNOMON_PROJECT->editorFileInfo({"lpy", "py"});

    for (auto f : lpy_files) {
        emit requestOpenFile(f);
    }
}

bool gnomonWorkspaceLSystemModel::readOnly() {
    if (d->open_files[d->file].contains(d->lpy_dir->path())) {
        return d->file != "vonKoch.lpy";
    } else {
        return GNOMON_PROJECT->isReadOnly(d->open_files[d->file]);
    }
}
