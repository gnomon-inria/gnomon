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

    gnomonAbstractFormAlgorithm *algorithm = nullptr;
    gnomonFormAlgorithmCommand *command = nullptr;
    QJsonObject state;
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

void gnomonWorkspacePythonAlgorithm::read(const QString& file_url)
{
    QString file_path;
    const QUrl url(file_url);
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    if (url.isLocalFile()) {
        file_path = QDir::toNativeSeparators(url.toLocalFile());
    } else {
        file_path = file_url;
    }

    QFile f(file_path);
    if (f.open(QIODevice::ReadOnly)) {
        settings.setValue("Python/load", file_path);
        QTextStream s(&f);
        d->code->setText(s.readAll());
        d->code->parseCode();
        emit d->code->codeUpdated();
        this->backup();
    } else {
        dtkWarn()<<"Could not open file"<<file_path;
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

QUrl gnomonWorkspacePythonAlgorithm::defaultReadPath(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    return settings.value("Python/load").toString();
}

void gnomonWorkspacePythonAlgorithm::run(void) {

    if(d->algorithm) {
        emit started();
        d->algorithm->setLogServerAddress(gnomonLogCaptureServer::instance()->completeAddress());
        d->algorithm->run();
        this->viewOutputs();
        emit finished();
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

        std::shared_ptr<gnomonBinaryImageSeries> binaryImage = d->algorithm->outputBinaryImage();
        if ((binaryImage) && (binaryImage->times().size() != 0)) {
            d->command->addOutput(binaryImage);
            this->target()->setForm("gnomonBinaryImage", binaryImage);
            QString form_name("binaryImage_out");
            if (d->code->outputForms().contains("gnomonBinaryImage")) {
                form_name = d->code->outputForms()["gnomonBinaryImage"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.outputBinaryImage(False).items()}",
                    &stat);
            output_form_added = true;
            GNOMON_SESSION->registerForm(binaryImage);
            int form_count = gnomonFormManager::instance()->formCount(binaryImage->formName());
            binaryImage->metadata()->set("name",
                                         binaryImage->formName().remove("gnomon") + QString::number(form_count + 1));
            binaryImage->metadata()->set("source", d->algorithm_key);
        }

        std::shared_ptr<gnomonCellComplexSeries> cellComplex = d->algorithm->outputCellComplex();
        if ((cellComplex) && (cellComplex->times().size() != 0)) {
            d->command->addOutput(cellComplex);
            this->target()->setForm("gnomonCellComplex", cellComplex);
            QString form_name("cellcomplex_out");
            if (d->code->outputForms().contains("gnomonCellComplex")) {
                form_name = d->code->outputForms()["gnomonCellComplex"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.outputCellComplex(False).items()}",
                    &stat);
            output_form_added = true;
            GNOMON_SESSION->registerForm(cellComplex);
            int form_count = gnomonFormManager::instance()->formCount(cellComplex->formName());
            cellComplex->metadata()->set("name",
                                         cellComplex->formName().remove("gnomon") + QString::number(form_count + 1));
            cellComplex->metadata()->set("source", d->algorithm_key);
        }

        std::shared_ptr<gnomonCellImageSeries> cellImage = d->algorithm->outputCellImage();
        if ((cellImage) && (cellImage->times().size() != 0)) {
            d->command->addOutput(cellImage);
            this->target()->setForm("gnomonCellImage", cellImage);
            QString form_name("cellimage_out");
            if (d->code->outputForms().contains("gnomonCellImage")) {
                form_name = d->code->outputForms()["gnomonCellImage"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.outputCellImage(False).items()}",
                    &stat);
            output_form_added = true;
            GNOMON_SESSION->registerForm(cellImage);
            int form_count = gnomonFormManager::instance()->formCount(cellImage->formName());
            cellImage->metadata()->set("name",
                                       cellImage->formName().remove("gnomon") + QString::number(form_count + 1));
            cellImage->metadata()->set("source", d->algorithm_key);
        }

        std::shared_ptr<gnomonImageSeries> image = d->algorithm->outputImage();
        if ((image) && (image->times().size() != 0) && (image->current()->channels().size() != 0)) {
            d->command->addOutput(image);
            this->target()->setForm("gnomonImage", image);
            QString form_name("image_out");
            if (d->code->outputForms().contains("gnomonImage")) {
                form_name = d->code->outputForms()["gnomonImage"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.outputImage(False).items()}",
                    &stat);
            output_form_added = true;
            GNOMON_SESSION->registerForm(image);
            int form_count = gnomonFormManager::instance()->formCount(image->formName());
            image->metadata()->set("name", image->formName().remove("gnomon") + QString::number(form_count + 1));
            image->metadata()->set("source", d->algorithm_key);
        }

        std::shared_ptr<gnomonLStringSeries> lString = d->algorithm->outputLString();
        if ((lString) && (lString->times().size() != 0)) {
            d->command->addOutput(lString);
            this->target()->setForm("gnomonLString", lString);
            QString form_name("lString_out");
            if (d->code->outputForms().contains("gnomonLString")) {
                form_name = d->code->outputForms()["gnomonLString"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.outputLString(False).items()}",
                    &stat);
            output_form_added = true;
            GNOMON_SESSION->registerForm(lString);
            int form_count = gnomonFormManager::instance()->formCount(lString->formName());
            lString->metadata()->set("name", lString->formName().remove("gnomon") + QString::number(form_count + 1));
            lString->metadata()->set("source", d->algorithm_key);
        }

        std::shared_ptr<gnomonMeshSeries> mesh = d->algorithm->outputMesh();
        if ((mesh) && (mesh->times().size() != 0)) {
            d->command->addOutput(mesh);
            this->target()->setForm("gnomonMesh", mesh);
            QString form_name("mesh_out");
            if (d->code->outputForms().contains("gnomonMesh")) {
                form_name = d->code->outputForms()["gnomonMesh"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.outputMesh(False).items()}",
                    &stat);
            output_form_added = true;
            GNOMON_SESSION->registerForm(mesh);
            int form_count = gnomonFormManager::instance()->formCount(mesh->formName());
            mesh->metadata()->set("name", mesh->formName().remove("gnomon") + QString::number(form_count + 1));
            mesh->metadata()->set("source", d->algorithm_key);
        }

        std::shared_ptr<gnomonPointCloudSeries> pointCloud = d->algorithm->outputPointCloud();
        if ((pointCloud) && (pointCloud->times().size() != 0)) {
            d->command->addOutput(pointCloud);
            this->target()->setForm("gnomonPointCloud", pointCloud);
            QString form_name("pointcloud_out");
            if (d->code->outputForms().contains("gnomonPointCloud")) {
                form_name = d->code->outputForms()["gnomonPointCloud"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(
                    form_name + " = {t:f.data().__data_getter() for t,f in algorithm.outputPointCloud(False).items()}",
                    &stat);
            output_form_added = true;
            GNOMON_SESSION->registerForm(pointCloud);
            int form_count = gnomonFormManager::instance()->formCount(pointCloud->formName());
            pointCloud->metadata()->set("name",
                                        pointCloud->formName().remove("gnomon") + QString::number(form_count + 1));
            pointCloud->metadata()->set("source", d->algorithm_key);
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
    unSerialize(d->state);
    for (auto view : d->sources->views()) {
        view->restoreState();
    }
    for (auto view : d->targets->views()) {
        view->restoreState();
    }
}

void gnomonWorkspacePythonAlgorithm::export_outputs(void) {
    for(const auto &output_view: d->targets->views()) {
        output_view->transmit();
    }
}

QJsonObject gnomonWorkspacePythonAlgorithm::serialize() {
    QJsonObject state;
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


    return state;
}

void gnomonWorkspacePythonAlgorithm::unSerialize(const QJsonObject &state) {
    d->code->setText(state["code"].toString());
    d->code->parseCode();
    setEditMode(state["edit"].toBool());

    QJsonObject parameters_json = state["parameters"].toObject();
    if(d->command && !parameters_json.empty()) {
        for(const auto& param_name: parameters_json.keys()) {
            auto param = parameters_json[param_name].toObject().toVariantHash();
            d->command->setParameter(param_name, dtkCoreParameter::create(param)->variant());
        }
    }
}

bool gnomonWorkspacePythonAlgorithm::backup(void) const
{
    return GNOMON_PROJECT->backupFile(d->code->fileName(), d->code->text());
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
