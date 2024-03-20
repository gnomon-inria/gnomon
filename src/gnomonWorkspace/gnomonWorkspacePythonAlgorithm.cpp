#include <gnomonCore>
#include "gnomonProject"
#include "gnomonWorkspacePythonAlgorithm.h"

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormAlgorithm>
#include <gnomonCore/gnomonCommand/gnomonFormAlgorithmCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <gnomonPipeline/gnomonPipelineManager.h>

#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>
#include <gnomonVisualization/gnomonView/gnomonVtkViewPool.h>

#include "gnomonPythonAlgorithmPluginCode.h"
#include "gnomonAlgorithmsLogs/gnomonLogCaptureServer"

#include <dtkScript>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePythonAlgorithmPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonWorkspacePythonAlgorithmPrivate
{
public:
    void loadAlgorithm(void);
    void registerPipeline(void);

public:
    gnomonPythonAlgorithmPluginCode *code = nullptr;
    QProcess *lsp_process = nullptr;

public:
    bool edit_mode = true;

public:
    gnomonVtkViewList *sources = nullptr;
    gnomonVtkViewList *targets = nullptr;
    gnomonVtkViewPool *pool = nullptr;

public:
    QString algorithm_key;
    QString object_key;

    QString current_file;
    bool read_only;
    QMap<QString, QString> open_files;

    gnomonAbstractFormAlgorithm *algorithm = nullptr;
    gnomonFormAlgorithmCommand *command = nullptr;
    QJsonObject state;

    QMetaObject::Connection editor_connect;

    QFutureWatcher<void> *watcher = nullptr;
    QMetaObject::Connection connect_finished;
};

void gnomonWorkspacePythonAlgorithmPrivate::loadAlgorithm(void)
{
    if (this->algorithm) {
        delete this->algorithm;
        this->algorithm = nullptr;
    }
    gnomonCore::formAlgorithm::pluginFactory().clear();

    int stat;
    QString output = dtkScriptInterpreterPython::instance()->interpret(this->code->text(), &stat);

    if (gnomonCore::formAlgorithm::pluginFactory().keys().size() > 0) {
        this->algorithm_key = gnomonCore::formAlgorithm::pluginFactory().keys()[0];
        this->algorithm = gnomonCore::formAlgorithm::pluginFactory().create(this->algorithm_key);
        Q_ASSERT(this->algorithm);
        this->code->parseCode();
    }

    if (this->algorithm) {
        dtkCoreObjectManager *object_manager = dtkCoreObjectManager::instance();
        int algo_id = 0;
        // TODO: Make sure that the gnomonPluginFactory actually registers to the dtkCoreObjectManager
        for (const auto& key : object_manager->keys()) {
            QRegularExpression rx("gnomonAbstractFormAlgorithm[*] ([0-9]*)");
            auto match = rx.match(key);
            if (match.hasMatch()) {
                int key_id = match.capturedTexts()[1].toInt();
                if (key_id > algo_id) {
                    this->object_key = key;
                    algo_id = key_id;
                }
            }
        }
    }
}

void gnomonWorkspacePythonAlgorithmPrivate::registerPipeline(void)
{
    if (this->command) {
        gnomonPipelineManager::instance()->addAlgorithm(this->command);
    }
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspacePythonAlgorithm
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspacePythonAlgorithm::gnomonWorkspacePythonAlgorithm(QObject *parent) : gnomonAbstractWorkspace(parent)
{
    d = new gnomonWorkspacePythonAlgorithmPrivate;

    QString lsp_server = "pylsp";
    QStringList arguments;
    arguments << "--ws" << "--host" << "127.0.0.1" << "--port" << "3000";

    d->lsp_process = new QProcess(parent);
    d->lsp_process->start(lsp_server, arguments);
    if(!d->lsp_process->waitForStarted()) {
        qWarning() << "Error launching Python LSP server";
    }


    d->code = new gnomonPythonAlgorithmPluginCode(this);
    d->code->updateCode();
    d->current_file = d->code->fileName();

    d->sources = new gnomonVtkViewList(this);
    d->targets = new gnomonVtkViewList(this);

    d->pool = new gnomonVtkViewPool(this);

    connect(d->sources, &gnomonVtkViewList::viewAdded, [=] (gnomonVtkView *v) {
        v->setAcceptForm("gnomonBinaryImage",true);
        v->setAcceptForm("gnomonCellComplex",true);
        v->setAcceptForm("gnomonCellImage",true);
        v->setAcceptForm("gnomonImage",true);
        v->setAcceptForm("gnomonLString",true);
        v->setAcceptForm("gnomonMesh",true);
        v->setAcceptForm("gnomonPointCloud",true);
        v->setInputView(true);
        connect(v, &gnomonVtkView::formAdded, [=] () {
            this->setInputs();
            emit parametersChanged();
        });
    });

    connect(d->targets, &gnomonVtkViewList::viewAdded, [=] (gnomonVtkView *v) {
        v->setAcceptForm("gnomonBinaryImage",true);
        v->setAcceptForm("gnomonCellComplex",true);
        v->setAcceptForm("gnomonCellImage",true);
        v->setAcceptForm("gnomonImage",true);
        v->setAcceptForm("gnomonLString",true);
        v->setAcceptForm("gnomonMesh",true);
        v->setAcceptForm("gnomonPointCloud",true);
        connect(v, &gnomonVtkView::exportedForm, [=] (std::shared_ptr<gnomonAbstractDynamicForm> f) {
            gnomonPipelineManager::instance()->addForm(f->uuid());
            this->m_can_be_destroyed = false;
            emit canBeDestroyedChanged(false);
        });
    });

    d->sources->addView();
    d->targets->addView();

    d->pool->addView(this->source());
    d->pool->addView(this->target());
}

gnomonWorkspacePythonAlgorithm::~gnomonWorkspacePythonAlgorithm(void)
{
    if(d->lsp_process) {
        d->lsp_process->kill();
        d->lsp_process = nullptr;
    }
    delete d;
}

void gnomonWorkspacePythonAlgorithm::loadAlgorithm(void)
{
    d->loadAlgorithm();
    emit algorithmLoaded();
    if (d->algorithm) {
        this->setInputs();
        emit parametersChanged();
    }
}

QString gnomonWorkspacePythonAlgorithm::algorithm(void) const
{
    return d->algorithm_key;
}

void gnomonWorkspacePythonAlgorithm::read(const QString& file_url, bool read_only)
{
    QString relative_path;
    const QUrl url(file_url);
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    if (url.isLocalFile()) {
        relative_path = QDir::toNativeSeparators(url.toLocalFile());
    } else {
        relative_path = file_url;
    }
    QString file_name = relative_path.split(QRegularExpression("/")).last();

    QString absolute_path;
    QString source;
    if (!QFile::exists(relative_path)) {
            qDebug() << Q_FUNC_INFO << "file " << relative_path << "doesn't exist";
    } else {
        absolute_path = GNOMON_PROJECT->findFile(relative_path);
        source = QFileInfo(relative_path).fileName();
    }

    bool to_copy = (!read_only) & (absolute_path=="");
    qDebug()<<Q_FUNC_INFO<<relative_path<<"["<<absolute_path<<"]"<<to_copy;

    if(to_copy) {
        QString project_file_path = GNOMON_PROJECT->projectDir() + "/" + file_name;
        if(!QFile::copy(relative_path, project_file_path)) {
            dtkWarn()<<"Failed to copy file "<< relative_path << "to Project";
            return;
        }
        relative_path = GNOMON_PROJECT->relativePath(project_file_path);
    }

    QFile f(relative_path);
    if (f.open(QIODevice::ReadOnly)) {
        settings.setValue("Python/load", relative_path);
        this->setFileName(file_name);
        QTextStream s(&f);
        d->code->setText(s.readAll());
        d->code->parseCode();
        d->open_files[d->code->fileName()] = relative_path;
        emit d->code->codeUpdated();
        emit stateChanged();
        if(!read_only)
            this->backup();
    } else {
        dtkWarn()<<"Could not open file"<<relative_path;
    }
}

void gnomonWorkspacePythonAlgorithm::save(const QString& file_url) const
{
    QString file_path;
    const QUrl url(file_url);
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    if(url.isLocalFile()) {
        file_path = QDir::toNativeSeparators(url.toLocalFile());
    } else {
        file_path = file_url;
    }

    if(!file_path.isEmpty()) {
        QFile f(file_path);
        if(f.open(QIODevice::WriteOnly| QIODevice::Text)) {
            QTextStream out(&f);
            out << d->code->text();
            settings.setValue("Python/load", file_path);
            f.close();
            emit d->code->codeUpdated();
            this->backup();
        } else {
            dtkWarn()<<"Could not save to file"<<file_path;
        }
    }
}

void gnomonWorkspacePythonAlgorithm::close(const QString& file_name)
{
    if (d->open_files.contains(file_name)) {
        d->open_files.remove(file_name);
        emit stateChanged();
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"The file"<<file_name<<"was not open";
    }
}

QString gnomonWorkspacePythonAlgorithm::fileName(void) const
{
    return d->current_file;
}

bool gnomonWorkspacePythonAlgorithm::readOnly(void) const
{
    return d->read_only;
}

void gnomonWorkspacePythonAlgorithm::setFileName(const QString& file_name)
{
    if (file_name != d->current_file) {
        d->code->setFileName(file_name);
        d->current_file = file_name;
        emit fileChanged(d->current_file);
    }
}

void gnomonWorkspacePythonAlgorithm::setReadOnly(bool read_only)
{
    if (read_only != d->read_only) {
        d->read_only = read_only;
        emit readOnlyChanged(d->read_only);
    }
}

void gnomonWorkspacePythonAlgorithm::importFile(const QString& file_name, const QString& path)
{
    if (d->open_files.contains(file_name)) {
        auto project_file = path + "/" + file_name;
        QFile::copy(d->open_files[file_name], project_file);
        QString relative_path = GNOMON_PROJECT->relativePath(project_file);
        d->open_files[file_name] = relative_path;
        this->backup();
        emit GNOMON_PROJECT->fileImported(relative_path);
    }
}

QUrl gnomonWorkspacePythonAlgorithm::defaultReadPath(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    return settings.value("Python/load").toString();
}

void gnomonWorkspacePythonAlgorithm::run(void) {

    if(d->algorithm) {
        disconnect(d->connect_finished);
        d->connect_finished = connect(d->command, &gnomonAbstractCommand::finished, [this]() {
            this->viewOutputs();
            emit finished();
        });
        emit started();
        d->command->redo();
    } else {
        dtkWarn() << Q_FUNC_INFO << "d->algorithm is null, nothing is done!";
    }

}

void gnomonWorkspacePythonAlgorithm::setInputs()
{
    if (d->command) {
        d->command->setFormAlgorithm(nullptr);
        d->command->disconnect();
        delete d->command;
        d->command = nullptr;
    }

    if (d->algorithm) {
        int stat;
        QString output;
        // TODO : make sure that we look into the right object manager
        output = dtkScriptInterpreterPython::instance()->interpret("from gnomon.core import objectManagerFormAlgorithm",
                                                                   &stat);
        output = dtkScriptInterpreterPython::instance()->interpret(
                "algorithm = objectManagerFormAlgorithm(\"" + d->object_key + "\")", &stat);

        d->command = new gnomonFormAlgorithmCommand(d->algorithm_key);
        d->command->setFormAlgorithm(d->algorithm);
        d->command->setPythonCode(d->code->text());
        connect(d->command, &gnomonFormAlgorithmCommand::finished, [this]() {
            this->viewOutputs();
            emit finished();
        }); // never called as the algo is run directly and not through the command

        if (this->source()->binaryImage()) {
            d->algorithm->setInputBinaryImage(this->source()->binaryImage());
            d->command->addInput(this->source()->binaryImage());
            QString form_name("binaryImage_in");
            if (d->code->inputForms().contains("gnomonBinaryImage")) {
                form_name = d->code->inputForms()["gnomonBinaryImage"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.inputBinaryImage(False).items()}",
                    &stat);
        }
        if (this->source()->cellComplex()) {
            d->algorithm->setInputCellComplex(this->source()->cellComplex());
            d->command->addInput(this->source()->cellComplex());
            QString form_name("cellcomplex_in");
            if (d->code->inputForms().contains("gnomonCellComplex")) {
                form_name = d->code->inputForms()["gnomonCellComplex"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.inputCellComplex(False).items()}",
                    &stat);
        }
        if (this->source()->cellImage()) {
            d->algorithm->setInputCellImage(this->source()->cellImage());
            d->command->addInput(this->source()->cellImage());
            QString form_name("cellimage_in");
            if (d->code->inputForms().contains("gnomonCellImage")) {
                form_name = d->code->inputForms()["gnomonCellImage"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.inputCellImage(False).items()}",
                    &stat);
        }
        if (this->source()->image()) {
            d->algorithm->setInputImage(this->source()->image());
            d->command->addInput(this->source()->image());
            QString form_name("image_in");
            if (d->code->inputForms().contains("gnomonImage")) {
                form_name = d->code->inputForms()["gnomonImage"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.inputImage(False).items()}",
                    &stat);
        }
        if (this->source()->lString()) {
            d->algorithm->setInputLString(this->source()->lString());
            d->command->addInput(this->source()->lString());
            QString form_name("lString_in");
            if (d->code->inputForms().contains("gnomonLString")) {
                form_name = d->code->inputForms()["gnomonLString"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.inputLString(False).items()}", &stat);
        }
        if (this->source()->mesh()) {
            d->algorithm->setInputMesh(this->source()->mesh());
            d->command->addInput(this->source()->mesh());
            QString form_name("mesh_in");
            if (d->code->inputForms().contains("gnomonMesh")) {
                form_name = d->code->inputForms()["gnomonMesh"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.inputMesh(False).items()}", &stat);
        }
        if (this->source()->pointCloud()) {
            d->algorithm->setInputPointCloud(this->source()->pointCloud());
            d->command->addInput(this->source()->pointCloud());
            QString form_name("poinntcloud_in");
            if (d->code->inputForms().contains("gnomonPointCloud")) {
                form_name = d->code->inputForms()["gnomonPointCloud"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.inputPointCloud(False).items()}",
                    &stat);
        }

        for (const auto &parameter_name: d->algorithm->parameters().keys()) {
            d->command->addParameter(parameter_name, d->algorithm->parameters()[parameter_name]);
        }
    }
}

void gnomonWorkspacePythonAlgorithm::viewOutputs(void)
{
    this->target()->clear();

    if (d->algorithm) {
        int stat;
        QString output;
        output = dtkScriptInterpreterPython::instance()->interpret("from gnomon.core import objectManagerFormAlgorithm",
                                                                   &stat);
        output = dtkScriptInterpreterPython::instance()->interpret(
                "algorithm = objectManagerFormAlgorithm(\"" + d->object_key + "\")", &stat);

        this->target()->clear();

        bool output_form_added = false;

        for (auto output_name: d->command->outputs().keys()) {
            auto form = d->command->outputs()[output_name];
            if ((form) && (form->times().size() != 0)) {
                QString form_name = form->formName();
                this->target()->setForm(form_name, form);
                QString form_variable = form_name.remove("gnomon") + "_out";
                form_variable = form_variable.left(1).toLower() + form_variable.mid(1);
                if (d->code->outputForms().contains("gnomonBinaryImage")) {
                    form_variable = d->code->outputForms()["gnomonBinaryImage"].name;
                }
                QString form_statement = form_variable + " = ";
                form_statement += "{t: f.data().__data_getter() for t,f in algorithm.output" + form_name.remove("gnomon") + "(False).items()}";
                output = dtkScriptInterpreterPython::instance()->interpret(form_statement, &stat);
                output_form_added = true;
                GNOMON_SESSION->trackForm(form);
                int form_count = gnomonFormManager::instance()->formCount(form_name);
                form->metadata()->set("name", form_name.remove("gnomon") + QString::number(form_count + 1));
                form->metadata()->set("source", d->algorithm_key);
            }
        }

        if (output_form_added) {
            this->target()->render();
            if(!this->target()->synced()) {
                this->target()->tryLinking();
            }
            d->registerPipeline();
        }
    }
}

bool gnomonWorkspacePythonAlgorithm::editMode(void)
{
    return d->edit_mode;
}

void gnomonWorkspacePythonAlgorithm::setEditMode(bool edit)
{
    if (edit != d->edit_mode) {
        d->edit_mode = edit;
        if (!d->edit_mode) {
            this->loadAlgorithm();
        }
        emit editModeChanged();
    }
}

gnomonPythonAlgorithmPluginCode* gnomonWorkspacePythonAlgorithm::code(void) const
{
    return d->code;
}

gnomonVtkViewList* gnomonWorkspacePythonAlgorithm::sources(void) const
{
    return d->sources;
}

gnomonVtkViewList* gnomonWorkspacePythonAlgorithm::targets(void) const
{
    return d->targets;
}

QJSValue gnomonWorkspacePythonAlgorithm::parameters(void)
{
    if (d->command) {
        return dtkCoreParameterCollection(d->command->parameters()).toJSValue(this);
    } else {
        return dtkCoreParameterCollection().toJSValue(this);
    }
}

bool gnomonWorkspacePythonAlgorithm::isEmpty(void)
{
    return false;
}

void gnomonWorkspacePythonAlgorithm::saveState(void)
{
    d->state = serialize();
}

void gnomonWorkspacePythonAlgorithm::restoreState(void)
{
    deserialize(d->state);
}

void gnomonWorkspacePythonAlgorithm::export_outputs(void) {
    for(const auto &output_view: d->targets->views()) {
        output_view->transmit();
    }
}

QJsonObject gnomonWorkspacePythonAlgorithm::serialize() {
    QJsonObject state = gnomonAbstractWorkspace::serialize();

    QJsonObject open_file_json;
    for (const auto& file_name : d->open_files.keys()) {
        open_file_json.insert(file_name, d->open_files[file_name]);
    }
    state.insert("open_files", open_file_json);

    state.insert("code", d->code->text());
    state.insert("algoName", d->algorithm_key);
    state.insert("edit", d->edit_mode);

    QVariantMap parameters_json;
    if(d->command) {
        dtkCoreParameters dtkParameters = d->command->parameters();
        for(const auto& param_name : dtkParameters.keys()){
            auto param_value = dtkParameters[param_name]->toVariantHash();
            parameters_json.insert(param_name, QJsonObject::fromVariantHash(param_value));
        }
    }
    state.insert("parameters", QJsonObject::fromVariantMap(parameters_json));
    QJsonArray sources;
    for (auto view : d->sources->views()) {
        sources.append(view->serialize());
    }
    QJsonArray targets;
    for (auto view : d->targets->views()) {
        targets.append(view->serialize());
    }
    state.insert("sources", sources);
    state.insert("targets", targets);

    return state;
}

void gnomonWorkspacePythonAlgorithm::deserialize(const QJsonObject &state) {
    gnomonAbstractWorkspace::deserialize(state);
    disconnect(d->editor_connect);

    QJsonObject open_file_json = state["open_files"].toObject();
    for (auto file_name: open_file_json.keys()) {
        QString file_path = open_file_json[file_name].toString();
        d->open_files[file_name] = file_path;
    }

    d->editor_connect = connect(this, &gnomonWorkspacePythonAlgorithm::codeEditorReady, [=] () {
        for (auto file_name: d->open_files.keys()) {
            emit requestOpenFile(d->open_files[file_name]);
        }
        d->code->parseCode();
    });

    // TODO: to remove if code is restored from file / backup ?
    // d->code->setText(state["code"].toString());
    setEditMode(state["edit"].toBool());

    QJsonObject parameters_json = state["parameters"].toObject();
    if(d->command && !parameters_json.empty()) {
        for(const auto& param_name: parameters_json.keys()) {
            auto param = parameters_json[param_name].toObject().toVariantHash();
            d->command->setParameter(param_name, dtkCoreParameter::create(param)->variant());
        }
    }
    QJsonArray sources = state.value("sources").toArray();
    int i = 0;
    for (auto view : d->sources->views()) {
        view->deserialize(sources[i].toObject());
        i++;
    }
    QJsonArray targets = state.value("targets").toArray();
    i = 0;
    for (auto view : d->targets->views()) {
        view->deserialize(targets[i].toObject());
        i++;
    }
}

bool gnomonWorkspacePythonAlgorithm::backup(void) const
{
    QString file_name = d->code->fileName();
    bool ok = false;
    if (d->open_files.contains(file_name)) {
        QString file_path = d->open_files[file_name];
        ok = GNOMON_PROJECT->backupFile(file_path, d->code->text());
    }
    return ok;
}

void gnomonWorkspacePythonAlgorithm::restore(void)
{
    QStringList py_files = GNOMON_PROJECT->editorFileInfo({"py"});

    for (auto f: py_files) {
        emit requestOpenFile(f);
    }
}

//
// gnomonWorkspacePythonAlgorithm.cpp ends here
