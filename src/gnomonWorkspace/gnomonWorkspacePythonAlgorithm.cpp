#include "gnomonWorkspacePythonAlgorithm.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonFormAlgorithmCommand>
#include <gnomonPipeline>
#include <gnomonVisualization>

#include <dtkCore>
#include <dtkScript>
#include <QFileDialog>

#include "gnomonPythonAlgorithmPluginCode.h"


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

public:
    bool edit_mode = true;

public:
    gnomonViewFormList *sources = nullptr;
    gnomonViewFormList *targets = nullptr;
    gnomonViewFormPool *pool = nullptr;

public:
    QString algorithm_key;
    QString object_key;

    gnomonAbstractFormAlgorithm *algorithm = nullptr;
    gnomonFormAlgorithmCommand *command = nullptr;
};

void gnomonWorkspacePythonAlgorithmPrivate::loadAlgorithm(void)
{
    gnomonCore::formAlgorithm::pluginFactory().clear();

    int stat;
    QString output = dtkScriptInterpreterPython::instance()->interpret(this->code->text(), &stat);

    if (gnomonCore::formAlgorithm::pluginFactory().keys().size() > 0) {
        this->algorithm_key = gnomonCore::formAlgorithm::pluginFactory().keys()[0];
        qDebug()<<Q_FUNC_INFO<<this->algorithm_key;
        this->algorithm = gnomonCore::formAlgorithm::pluginFactory().create(this->algorithm_key);
        Q_ASSERT(this->algorithm);
        this->code->parseCode();
    } else {
        this->algorithm = nullptr;
    }

    if (this->algorithm) {
        dtkCoreObjectManager *object_manager = dtkCoreObjectManager::instance();
        int algo_id = 0;
        for (const auto& key : object_manager->keys()) {
            QRegExp rx("gnomonAbstractFormAlgorithm[*] ([0-9]*)");
            int pos = rx.indexIn(key);
            if (pos != -1) {
                int key_id = rx.capturedTexts()[1].toInt();
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
        gnomonAbstractCommand *algorithm_command = dynamic_cast<gnomonAbstractCommand *>(this->command);
        gnomonPipeline::instance()->addAlgorithm(algorithm_command);
    }
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspacePythonAlgorithm
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspacePythonAlgorithm::gnomonWorkspacePythonAlgorithm(QObject *parent) : QObject(parent)
{
    d = new gnomonWorkspacePythonAlgorithmPrivate;

    d->code = new gnomonPythonAlgorithmPluginCode(this);
    d->code->updateCode();

    d->sources = new gnomonViewFormList(this);
    d->targets = new gnomonViewFormList(this);

    d->pool = new gnomonViewFormPool(this);

    connect(d->sources, &gnomonViewFormList::viewAdded, [=] (gnomonViewForm *v) {
        v->setAcceptForm("gnomonCellComplex",true);
        v->setAcceptForm("gnomonCellImage",true);
        v->setAcceptForm("gnomonImage",true);
        v->setAcceptForm("gnomonMesh",true);
        v->setAcceptForm("gnomonPointCloud",true);
        v->setInputView(true);
    });

    connect(d->targets, &gnomonViewFormList::viewAdded, [=] (gnomonViewForm *v) {
        v->setAcceptForm("gnomonCellComplex",true);
        v->setAcceptForm("gnomonCellImage",true);
        v->setAcceptForm("gnomonImage",true);
        v->setAcceptForm("gnomonMesh",true);
        v->setAcceptForm("gnomonPointCloud",true);
        connect(v, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), gnomonPipeline::instance(), SLOT(addForm(gnomonAbstractDynamicForm *)));

    });

    d->sources->addView();
    d->targets->addView();

    d->pool->addView(this->source());
    d->pool->addView(this->target());
}

gnomonWorkspacePythonAlgorithm::~gnomonWorkspacePythonAlgorithm(void)
{
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
    if (url.isLocalFile()) {
        file_path = QDir::toNativeSeparators(url.toLocalFile());
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
        settings.setValue("Python/load", url);
    } else {
        file_path = file_url;
    }

    QFile f(file_path);
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream s(&f);
        d->code->setText(s.readAll());
        d->code->parseCode();
    }
}

void gnomonWorkspacePythonAlgorithm::save(const QString& file_url) const
{
    const QUrl url(file_url);
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    QString old_path = settings.value("Python/save", QDir::toNativeSeparators(url.toLocalFile())).toString();
    QString file_path = QFileDialog::getSaveFileName(nullptr,
                                            tr("Save Python File"),
                                            old_path,
                                            tr("Python (*.py)"));
    if(!file_path.isEmpty()) {
        QFile f(file_path);
        if(f.open(QIODevice::WriteOnly| QIODevice::Text)) {
            QTextStream out(&f);
            out << d->code->text();
            settings.setValue("Python/save", file_path);
            f.close();
        } else {
            qWarning() << "couldn t save to file" << file_path;
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
        this->setInputs();
        d->algorithm->run();
        this->viewOutputs();
    } else {
        dtkWarn() << Q_FUNC_INFO << "d->algorithm is null, nothing is done!";
    }

}

void gnomonWorkspacePythonAlgorithm::setInputs()
{
    if (d->command) {
        delete d->command;
        d->command = nullptr;
    }
    
    Q_ASSERT(d->algorithm);

    int stat;
    QString output;
    output = dtkScriptInterpreterPython::instance()->interpret("from gnomoncore import objectManagerFormAlgorithm", &stat);
    output = dtkScriptInterpreterPython::instance()->interpret("algorithm = objectManagerFormAlgorithm(\"" + d->object_key + "\")", &stat);

    d->command = new gnomonFormAlgorithmCommand(d->algorithm_key);
    
    if (this->source()->binaryImage()) {
        d->algorithm->setInputBinaryImage(this->source()->binaryImage());
        d->command->addInput(this->source()->binaryImage());
        QString form_name("binaryImage_in");
        if (d->code->inputForms().contains("gnomonBinaryImage")) {
            form_name = d->code->inputForms()["gnomonBinaryImage"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.inputBinaryImage(False)", &stat);
    }
    if (this->source()->cellComplex()) {
        d->algorithm->setInputCellComplex(this->source()->cellComplex());
        d->command->addInput(this->source()->cellComplex());
        QString form_name("cellcomplex_in");
        if (d->code->inputForms().contains("gnomonCellComplex")) {
            form_name = d->code->inputForms()["gnomonCellComplex"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.inputCellComplex(False)", &stat);
    }
    if (this->source()->cellImage()) {
        d->algorithm->setInputCellImage(this->source()->cellImage());
        d->command->addInput(this->source()->cellImage());
        QString form_name("cellimage_in");
        if (d->code->inputForms().contains("gnomonCellImage")) {
            form_name = d->code->inputForms()["gnomonCellImage"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.inputCellImage(False)", &stat);
    }
    if (this->source()->image()) {
        d->algorithm->setInputImage(this->source()->image());
        d->command->addInput(this->source()->image());
        QString form_name("image_in");
        if (d->code->inputForms().contains("gnomonImage")) {
            form_name = d->code->inputForms()["gnomonImage"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.inputImage(False)", &stat);
    }
    if (this->source()->mesh()) {
        d->algorithm->setInputMesh(this->source()->mesh());
        d->command->addInput(this->source()->mesh());
        QString form_name("mesh_in");
        if (d->code->inputForms().contains("gnomonMesh")) {
            form_name = d->code->inputForms()["gnomonMesh"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.inputMesh(False)", &stat);
    }
    if (this->source()->pointCloud()) {
        d->algorithm->setInputPointCloud(this->source()->pointCloud());
        d->command->addInput(this->source()->pointCloud());
        QString form_name("poinntcloud_in");
        if (d->code->inputForms().contains("gnomonPointCloud")) {
            form_name = d->code->inputForms()["gnomonPointCloud"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.inputPointCloud(False)", &stat);
    }

    for (const auto &parameter_name: d->algorithm->parameters().keys()) {
        d->command->addParameter(parameter_name, d->algorithm->parameters()[parameter_name]);
    }
}

void gnomonWorkspacePythonAlgorithm::viewOutputs(void)
{
    Q_ASSERT(d->algorithm);

    int stat;
    QString output;
    output = dtkScriptInterpreterPython::instance()->interpret("from gnomoncore import objectManagerFormAlgorithm", &stat);
    output = dtkScriptInterpreterPython::instance()->interpret("algorithm = objectManagerFormAlgorithm(\"" + d->object_key + "\")", &stat);

    this->target()->clear();

    bool output_form_added = false;

    gnomonBinaryImageSeries *binaryImage = d->algorithm->outputBinaryImage();
    if ((binaryImage) && (binaryImage->times().size() != 0)) {
        d->command->addOutput(binaryImage);
        this->target()->setForm("gnomonBinaryImage", binaryImage);
        QString form_name("binaryImage_out");
        if (d->code->outputForms().contains("gnomonBinaryImage")) {
            form_name = d->code->outputForms()["gnomonBinaryImage"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.outputBinaryImage(False)", &stat);
        output_form_added = true;
    }
    
    gnomonCellComplexSeries *cellComplex = d->algorithm->outputCellComplex();
    if ((cellComplex) && (cellComplex->times().size() != 0)) {
        d->command->addOutput(cellComplex);
        this->target()->setForm("gnomonCellComplex", cellComplex);
        QString form_name("cellcomplex_out");
        if (d->code->outputForms().contains("gnomonCellComplex")) {
            form_name = d->code->outputForms()["gnomonCellComplex"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.outputCellComplex(False)", &stat);
        output_form_added = true;
    }

    gnomonCellImageSeries *cellImage = d->algorithm->outputCellImage();
    if ((cellImage) && (cellImage->times().size() != 0)) {
        d->command->addOutput(cellImage);
        this->target()->setForm("gnomonCellImage", cellImage);
        QString form_name("cellimage_out");
        if (d->code->outputForms().contains("gnomonCellImage")) {
            form_name = d->code->outputForms()["gnomonCellImage"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.outputCellImage(False)", &stat);
        output_form_added = true;
    }

    gnomonImageSeries *image = d->algorithm->outputImage();
    if ((image) && (image->times().size() != 0) && (((gnomonImage *) image->current())->channels().size() != 0)) {
        d->command->addOutput(image);
        this->target()->setForm("gnomonImage", image);
        QString form_name("image_out");
        if (d->code->outputForms().contains("gnomonImage")) {
            form_name = d->code->outputForms()["gnomonImage"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.outputImage(False)", &stat);
        output_form_added = true;
    }

    gnomonMeshSeries *mesh = d->algorithm->outputMesh();
    if ((mesh) && (mesh->times().size() != 0)) {
        d->command->addOutput(mesh);
        this->target()->setForm("gnomonMesh", mesh);
        QString form_name("mesh_out");
        if (d->code->outputForms().contains("gnomonMesh")) {
            form_name = d->code->outputForms()["gnomonMesh"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.outputMesh(False)", &stat);
        output_form_added = true;
    }

    gnomonPointCloudSeries *pointCloud = d->algorithm->outputPointCloud();
    if ((pointCloud) && (pointCloud->times().size() != 0)) {
        d->command->addOutput(pointCloud);
        this->target()->setForm("gnomonPointCloud", pointCloud);
        QString form_name("pointcloud_out");
        if (d->code->outputForms().contains("gnomonPointCloud")) {
            form_name = d->code->outputForms()["gnomonPointCloud"].name;
        }
        output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.outputPointCloud(False)", &stat);
        output_form_added = true;
    }

    if (output_form_added) {
        this->target()->render();
        d->registerPipeline();
    }
}

bool gnomonWorkspacePythonAlgorithm::editMode(void)
{
    return d->edit_mode;
}

void gnomonWorkspacePythonAlgorithm::setEditMode(bool edit)
{
    bool changed = edit != d->edit_mode;
    d->edit_mode = edit;
    if (changed)
        emit editModeChanged();
}

gnomonPythonAlgorithmPluginCode* gnomonWorkspacePythonAlgorithm::code(void) const
{
    return d->code;
}

gnomonViewFormList* gnomonWorkspacePythonAlgorithm::sources(void) const
{
    return d->sources;
}

gnomonViewFormList* gnomonWorkspacePythonAlgorithm::targets(void) const
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


//
// gnomonWorkspacePythonAlgorithm.cpp ends here
