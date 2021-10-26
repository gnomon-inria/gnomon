// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonWorkspacePythonAlgorithm.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonFormAlgorithmCommand>
#include <gnomonPipeline>
#include <gnomonVisualization>

#include <dtkCore>
#include <dtkScript>
#include <dtkFonts>
#include <dtkMacs>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

#include "gnomonPythonAlgorithmPluginEditor.h"


// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePythonAlgorithmPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonWorkspacePythonAlgorithmPrivate
{
public:
    dtkWidgetsMenu *menu(dtkWidgetsWorkspace *);
    void configure(void);

public:
    void registerPipeline(void);

public:
    gnomonPythonAlgorithmPluginEditor *editor = nullptr;

public:
    dtkWidgetsMenuBarContainer *dashboard;
    gnomonInterpreterJupyter *terminal;

public:
    gnomonViewForm *source;
    gnomonViewForm *target;

    gnomonViewFormPool *pool = nullptr;

    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

public:
    QFormLayout *layout = nullptr;
    QHBoxLayout *viewer_layout = nullptr;

public:
    QHash<QString, dtkCoreParameter *> parameters;

public:
    QString algorithm_key;
    QString object_key;
    gnomonAbstractFormAlgorithm *algorithm = nullptr;
    gnomonFormAlgorithmCommand *command = nullptr;
};

dtkWidgetsMenu *gnomonWorkspacePythonAlgorithmPrivate::menu(dtkWidgetsWorkspace *parent)
{
    QObject::connect(this->editor, &gnomonPythonAlgorithmPluginEditor::openButtonClicked, [=] () {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");

        QString file_path;
        file_path = QFileDialog::getOpenFileName(parent, "Load Python script", settings.value("Python/load", ".").toString(), "Python scripts (*.py)");

        QFile f(file_path);
        if(f.open(QIODevice::ReadOnly)) {
            QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
            settings.setValue("Python/load", file_path);

            QTextStream s(&f);
            this->editor->setText(s.readAll());

            this->configure();
        }
    });

    QObject::connect(this->editor, &gnomonPythonAlgorithmPluginEditor::loadButtonClicked, [=] () {
        this->configure();
    });

    this->layout = new QFormLayout;

    QWidget *pane_item_parameters = new QWidget;
    pane_item_parameters->setLayout(this->layout);

    dtkWidgetsMenuItemDIY *menu_parameters = new dtkWidgetsMenuItemDIY("Parameters");
    menu_parameters->addWidget(pane_item_parameters);
    menu_parameters->setSizePolicy(QSizePolicy::Expanding);

    QPushButton *pane_item_run = new QPushButton("Apply");
    pane_item_run->setCheckable(true);

    dtkWidgetsMenuItemDIY *menu_button = new dtkWidgetsMenuItemDIY("Python Algorithm", pane_item_run);
    menu_button->setShowTitle(false);
    menu_button->setSizePolicy(QSizePolicy::Expanding);

    dtkWidgetsMenu *pane = new dtkWidgetsMenu(fa::circlethin, "Python Algorithm");
    pane->addItem(menu_parameters);
    pane->addItem(menu_button);

    QObject::connect(pane_item_run, &QPushButton::clicked, [=] () {
        dtkApp->window()->setCursor(Qt::BusyCursor);
        dynamic_cast<gnomonWorkspacePythonAlgorithm*>(parent)->run();
        dtkApp->window()->setCursor(Qt::ArrowCursor);
    });

    return pane;
}

void gnomonWorkspacePythonAlgorithmPrivate::configure(void)
{
    gnomonCore::formAlgorithm::pluginFactory().clear();

    int stat;
    QString output = dtkScriptInterpreterPython::instance()->interpret(this->editor->text(), &stat);

    if (gnomonCore::formAlgorithm::pluginFactory().keys().size() > 0) {
        this->algorithm_key = gnomonCore::formAlgorithm::pluginFactory().keys()[0];
        qDebug()<<Q_FUNC_INFO<<this->algorithm_key;
        this->algorithm = gnomonCore::formAlgorithm::pluginFactory().create(this->algorithm_key);
        Q_ASSERT(this->algorithm);
        this->editor->parseCode();
    } else {
        this->algorithm = nullptr;
    }

    if (this->layout) {
        for(int row = 0, max_row = this->layout->count(); row < max_row; ++row) {
            QLayoutItem *forDeletion = this->layout->takeAt(0);
            forDeletion->widget()->disconnect();
            delete forDeletion->widget();
            delete forDeletion;
        }
    } else {
        this->layout = new QFormLayout;
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

        dtkCoreParameters parameters = this->algorithm->parameters();
        QList<QString> keys = parameters.keys();
        keys.sort();
        for(QString k : keys) {
            QWidget *widget = gnomonWidgetsParameter::widget(parameters[k], 0);
            if (widget) {
                this->layout->addRow(k, widget);
            }
        }
//        this->layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    }
}

void gnomonWorkspacePythonAlgorithmPrivate::registerPipeline(void)
{
    if (this->command) {
        gnomonAbstractAlgorithmCommand *algorithm_command = dynamic_cast<gnomonAbstractAlgorithmCommand *>(this->command);
        gnomonPipeline::instance()->addAlgorithm(algorithm_command);
    }
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspacePythonAlgorithm::gnomonWorkspacePythonAlgorithm(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspacePythonAlgorithmPrivate;

    d->editor = new gnomonPythonAlgorithmPluginEditor(this);
    d->editor->resize(this->width(), 1000);

    d->editor->updateCode();

//    QVBoxLayout *editor_layout = new QVBoxLayout();
//    QWidget *editor_widget = new QWidget(this);
//    editor_widget->setLayout(editor_layout);
//
//    editor_layout->addWidget(d->editor);


// /////////////////////////////////////////////////////////////////////////////
// NOTE: Dashboard inception
// /////////////////////////////////////////////////////////////////////////////

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();

    d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->menu(this));
    d->dashboard->setFixedWidth(300);

    d->terminal = new gnomonInterpreterJupyter(this);
//    d->terminal->registerInterpreter(dtkScriptInterpreterPython::instance());
//    editor_layout->addWidget(d->terminal);

//    QSplitter *editor_splitter = new QSplitter(this);
//    editor_splitter->setOrientation(Qt::Horizontal);
//    editor_splitter->addWidget(d->editor);
//    editor_splitter->addWidget(d->terminal);

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(this->color);
    d->source->setAcceptForm("gnomonCellComplex",true);
    d->source->setAcceptForm("gnomonCellImage",true);
    d->source->setAcceptForm("gnomonImage",true);
    d->source->setAcceptForm("gnomonMesh",true);
    d->source->setAcceptForm("gnomonPointCloud",true);
    d->source->setInputView(true);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(this->color);
    d->target->setAcceptForm("gnomonCellComplex",true);
    d->target->setAcceptForm("gnomonCellImage",true);
    d->target->setAcceptForm("gnomonImage",true);
    d->target->setAcceptForm("gnomonMesh",true);
    d->target->setAcceptForm("gnomonPointCloud",true);
    connect(d->target, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), gnomonPipeline::instance(), SLOT(addForm(gnomonAbstractDynamicForm *)));

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);
    d->pool->linkAll();

    // /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

    d->target_message = new gnomonMessageBoard(this);
    d->target_message->setMessage("Result will be displayed here");

    d->target_stack = new QStackedWidget(this);
    d->target_stack->addWidget(d->target);
    d->target_stack->addWidget(d->target_message);
    d->target_stack->setCurrentWidget(d->target_message);

    // -- Organizing the viewer column --


    d->viewer_layout = new QHBoxLayout;
    d->viewer_layout->setContentsMargins(0, 0, 0, 0);
    d->viewer_layout->setSpacing(0);
    d->viewer_layout->addWidget(d->source);
    d->viewer_layout->addWidget(d->target_stack);

    QWidget *viewer = new QWidget(this);
    viewer->setLayout(d->viewer_layout);

    QSplitter *viewer_splitter = new QSplitter(this);
    viewer_splitter->setOrientation(Qt::Vertical);
    viewer_splitter->addWidget(viewer);
    viewer_splitter->addWidget(d->terminal);

//     // -- Organizing the whole workspace --
//    QSplitter *splitter = new QSplitter(this);
//    splitter->setOrientation(Qt::Horizontal);
//    splitter->addWidget(editor_splitter);
//    splitter->addWidget(viewer);

    QStackedWidget *editor_viewer_stack = new QStackedWidget(this);
    editor_viewer_stack->addWidget(d->editor);
    editor_viewer_stack->addWidget(viewer_splitter);
    editor_viewer_stack->setCurrentWidget(d->editor);

    QToolButton *editor_button = new QToolButton(this);
    editor_button->setIcon(dtkFontAwesome::instance()->icon(fa::edit));
    editor_button->setToolTip("Edit code");
    connect(editor_button, &QToolButton::clicked, [=] ()
    {
        editor_viewer_stack->setCurrentWidget(d->editor);
    });

    QToolButton *viewer_button = new QToolButton(this);
    viewer_button->setIcon(dtkFontAwesome::instance()->icon(fa::play));
    viewer_button->setToolTip("Run Python algorithm");
    connect(viewer_button, &QToolButton::clicked, [=] (void) -> void
    {
        d->configure();
        editor_viewer_stack->setCurrentWidget(viewer_splitter);
    });

    QHBoxLayout *button_layout = new QHBoxLayout;
    button_layout->addWidget(editor_button);
    button_layout->addWidget(viewer_button);
    button_layout->addStretch();

    QVBoxLayout *editor_viewer_layout = new QVBoxLayout;
    editor_viewer_layout->setContentsMargins(0, 0, 0, 0);
    editor_viewer_layout->setSpacing(0);
    editor_viewer_layout->addLayout(button_layout);
    editor_viewer_layout->addWidget(editor_viewer_stack);

    QWidget *editor_viewer = new QWidget(this);
    editor_viewer->setLayout(editor_viewer_layout);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(editor_viewer);
    layout->addWidget(d->dashboard);
}

gnomonWorkspacePythonAlgorithm::~gnomonWorkspacePythonAlgorithm(void)
{
    delete d;
}

void gnomonWorkspacePythonAlgorithm::resizeEvent(QResizeEvent *event)
{
    if (d->target_stack->currentWidget() == d->target_message)
    {
        d->target_stack->setCurrentWidget(d->target);
        d->target_message->setMinimumHeight(d->target->height());
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspacePythonAlgorithm::enter(void)
{
}

void gnomonWorkspacePythonAlgorithm::leave(void)
{
}

void gnomonWorkspacePythonAlgorithm::apply(void)
{
    d->configure();
}

void gnomonWorkspacePythonAlgorithm::run(void)
{
    d->target_stack->setCurrentWidget(d->target_message);
    d->source->setEnableLinking(false);
    d->target->setEnableLinking(false);

    if (d->command) {
        delete d->command;
        d->command = nullptr;
    }

    if (d->algorithm) {
        int stat;
        QString output;
        output = dtkScriptInterpreterPython::instance()->interpret("from gnomoncore import objectManagerFormAlgorithm", &stat);
        output = dtkScriptInterpreterPython::instance()->interpret("algorithm = objectManagerFormAlgorithm(\"" + d->object_key + "\")", &stat);

        d->command = new gnomonFormAlgorithmCommand(d->algorithm_key);

        if (d->source->cellComplex()) {
            d->algorithm->setInputCellComplex(d->source->cellComplex());
            d->command->addInput(d->source->cellComplex());
            QString form_name("cellcomplex_in");
            if (d->editor->inputForms().contains("gnomonCellComplex")) {
                form_name = d->editor->inputForms()["gnomonCellComplex"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.inputCellComplex(False)", &stat);
        }
        if (d->source->cellImage()) {
            d->algorithm->setInputCellImage(d->source->cellImage());
            d->command->addInput(d->source->cellImage());
            QString form_name("cellimage_in");
            if (d->editor->inputForms().contains("gnomonCellImage")) {
                form_name = d->editor->inputForms()["gnomonCellImage"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.inputCellImage(False)", &stat);
        }
        if (d->source->image()) {
            d->algorithm->setInputImage(d->source->image());
            d->command->addInput(d->source->image());
            QString form_name("image_in");
            if (d->editor->inputForms().contains("gnomonImage")) {
                form_name = d->editor->inputForms()["gnomonImage"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.inputImage(False)", &stat);
        }
        if (d->source->mesh()) {
            d->algorithm->setInputMesh(d->source->mesh());
            d->command->addInput(d->source->mesh());
            QString form_name("mesh_in");
            if (d->editor->inputForms().contains("gnomonMesh")) {
                form_name = d->editor->inputForms()["gnomonMesh"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.inputMesh(False)", &stat);
        }
        if (d->source->pointCloud()) {
            d->algorithm->setInputPointCloud(d->source->pointCloud());
            d->command->addInput(d->source->pointCloud());
            QString form_name("poinntcloud_in");
            if (d->editor->inputForms().contains("gnomonPointCloud")) {
                form_name = d->editor->inputForms()["gnomonPointCloud"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.inputPointCloud(False)", &stat);
        }

        for (const auto& parameter_name : d->algorithm->parameters().keys()){
            d->command->addParameter(parameter_name, d->algorithm->parameters()[parameter_name]);
        }

        d->algorithm->run();

        d->target->clear();
        d->target_stack->setCurrentWidget(d->target_message);

        bool output_form_added = false;

        gnomonCellComplexSeries *cellComplex = d->algorithm->outputCellComplex();
        if ((cellComplex)&&(cellComplex->times().size()!=0)) {
            d->command->addOutput(cellComplex);
            d->target->setForm("gnomonCellComplex",cellComplex);
            QString form_name("cellcomplex_out");
            if (d->editor->outputForms().contains("gnomonCellComplex")) {
                form_name = d->editor->outputForms()["gnomonCellComplex"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.outputCellComplex(False)", &stat);
            output_form_added = true;
        }

        gnomonCellImageSeries *cellImage = d->algorithm->outputCellImage();
        if ((cellImage)&&(cellImage->times().size()!=0)) {
            d->command->addOutput(cellImage);
            d->target->setForm("gnomonCellImage",cellImage);
            QString form_name("cellimage_out");
            if (d->editor->outputForms().contains("gnomonCellImage")) {
                form_name = d->editor->outputForms()["gnomonCellImage"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.outputCellImage(False)", &stat);
            output_form_added = true;
        }

        gnomonImageSeries *image = d->algorithm->outputImage();
         if ((image)&&(image->times().size()!=0)&&(((gnomonImage *)image->current())->channels().size()!=0)) {
            d->command->addOutput(image);
            d->target->setForm("gnomonImage",image);
            QString form_name("image_out");
            if (d->editor->outputForms().contains("gnomonImage")) {
                form_name = d->editor->outputForms()["gnomonImage"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.outputImage(False)", &stat);
            output_form_added = true;
        }

        gnomonMeshSeries *mesh = d->algorithm->outputMesh();
        if ((mesh)&&(mesh->times().size()!=0)) {
            d->command->addOutput(mesh);
            d->target->setForm("gnomonMesh",mesh);
            QString form_name("mesh_out");
            if (d->editor->outputForms().contains("gnomonMesh")) {
                form_name = d->editor->outputForms()["gnomonMesh"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.outputMesh(False)", &stat);
            output_form_added = true;
        }

        gnomonPointCloudSeries *pointCloud = d->algorithm->outputPointCloud();
        if ((pointCloud)&&(pointCloud->times().size()!=0)) {
            d->command->addOutput(pointCloud);
            d->target->setForm("gnomonPointCloud",pointCloud);
            QString form_name("pointcloud_out");
            if (d->editor->outputForms().contains("gnomonPointCloud")) {
                form_name = d->editor->outputForms()["gnomonPointCloud"].name;
            }
            output = dtkScriptInterpreterPython::instance()->interpret(form_name + " = algorithm.outputPointCloud(False)", &stat);
            output_form_added = true;
        }

        if (output_form_added) {
            d->target->render();
            d->target_stack->setCurrentWidget(d->target);
            d->source->setEnableLinking(true);
            d->target->setEnableLinking(true);
        }
    }

    if (d->target_stack->currentWidget() == d->target) {
        qDebug()<<Q_FUNC_INFO<<"Register Pipeline!";
        d->registerPipeline();
    }
}

const QColor gnomonWorkspacePythonAlgorithm::color = QColor("#a38948");

bool gnomonWorkspacePythonAlgorithm::isEmpty(void)
{
    return false;
}


//
// gnomonWorkspacePythonAlgorithm.cpp ends here
