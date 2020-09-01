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
#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkCore>
#include <dtkScript>
#include <dtkFonts>
#include <dtkMacs>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

// ///////////////////////////////////////////////////////////////////
// gnomonFormDescription
// ///////////////////////////////////////////////////////////////////

class gnomonFormDescription
{
public:
     gnomonFormDescription(const QString& name, const QString& type, const QString& data_plugin);
    ~gnomonFormDescription(void);

public:
    QString name;
    QString type;
    QString data_plugin;
};

gnomonFormDescription::gnomonFormDescription(const QString& name, const QString& type, const QString& data_plugin)
{
    this->name = name;
    this->type = type;
    this->data_plugin = data_plugin;
}

gnomonFormDescription::~gnomonFormDescription(void)
{
}

// ///////////////////////////////////////////////////////////////////
// gnomonPythonScriptEditor
// ///////////////////////////////////////////////////////////////////

class gnomonPythonScriptEditor : public QWidget
{
    Q_OBJECT

public:
     gnomonPythonScriptEditor(QWidget *parent = Q_NULLPTR);
    ~gnomonPythonScriptEditor(void);

protected:
    void resizeEvent(QResizeEvent *);

public:
    dtkMacsWidget *editor = nullptr;
    QVBoxLayout *layout = nullptr;
    dtkWidgetsMenuBar* script_menubar;

signals:
    void openButtonClicked(void);
    void saveButtonClicked(void);
    void loadButtonClicked(void);

public:
    QSize sizeHint(void) const;
};

gnomonPythonScriptEditor::gnomonPythonScriptEditor(QWidget *parent) : QWidget(parent)
{
    this->editor = new dtkMacsWidget(this);

    // -- Organizing the editor column --
    this->layout = new QVBoxLayout;
    this->layout->setContentsMargins(40, 0, 0, 0);
    this->layout->setSpacing(0);
    this->layout->addWidget(this->editor);

    this->script_menubar = new dtkWidgetsMenuBar(this);
    this->script_menubar->setInteractive(false);
    this->script_menubar->setStandalone(true);
    this->script_menubar->setWidth(32);
    this->script_menubar->setMargins(6);

    this->script_menubar->addMenu(fa::folder, "Open Python Script");
    this->script_menubar->addMenu(fa::save, "Save Python Script");
    this->script_menubar->addMenu(fa::play, "Load Python Script");

    connect(this->script_menubar, &dtkWidgetsMenuBar::clicked, [=] (int i_style)
    {
        if (i_style==0) {
            emit openButtonClicked();
        } else if (i_style==1) {
            emit saveButtonClicked();
        } else if (i_style==2) {
            emit loadButtonClicked();
        }

    });
//    this->script_menubar->move(QPoint(0, 0));
    this->script_menubar->touch();

    this->setLayout(this->layout);
}

gnomonPythonScriptEditor::~gnomonPythonScriptEditor(void)
{
    delete this->editor;
}

void gnomonPythonScriptEditor::resizeEvent(QResizeEvent *event)
{
    if (this->script_menubar) {
        this->script_menubar->setFixedHeight(event->size().height());
        this->script_menubar->touch();
    }

    QWidget::resizeEvent(event);
}

QSize gnomonPythonScriptEditor::sizeHint(void) const
{
    return QSize(1000, 800);
}

// ///////////////////////////////////////////////////////////////////
// gnomonPythonAlgorithmPluginEditor
// ///////////////////////////////////////////////////////////////////

class gnomonPythonAlgorithmPluginEditor : public gnomonPythonScriptEditor
{
    Q_OBJECT

public:
     gnomonPythonAlgorithmPluginEditor(QWidget *parent = Q_NULLPTR);
    ~gnomonPythonAlgorithmPluginEditor(void);

public:
    gnomonFormDescription *formDescriptionDialog(bool input = true);

public slots:
    void updateDataPlugins(const QString& form_type);
    void addInputForm(void);
    void addOutputForm(void);

public:
    QList<gnomonFormDescription *> input_forms;
    QList<gnomonFormDescription *> output_forms;

public:
    dtkWidgetsMenuBarContainer *form_pane;
    dtkWidgetsMenu *input_menu;
    dtkWidgetsMenu *output_menu;

    dtkWidgetsMenuBarContainer *parameter_pane;
    dtkWidgetsMenu *parameter_menu;

public:
    QComboBox *type_edit = nullptr;
    QComboBox *data_plugin_edit = nullptr;
};

gnomonPythonAlgorithmPluginEditor::gnomonPythonAlgorithmPluginEditor(QWidget *parent) : gnomonPythonScriptEditor(parent)
{
    QHBoxLayout *menu_layout = new QHBoxLayout;
    menu_layout->setContentsMargins(0, 0, 0, 0);

    this->input_menu = new dtkWidgetsMenu(fa::arrowcircledown, "Input Forms");
    dtkWidgetsMenuItem *add_input = this->input_menu->addItem(fa::plus,"Add input...");
    connect(add_input, &dtkWidgetsMenuItem::clicked,this, &gnomonPythonAlgorithmPluginEditor::addInputForm);

    this->output_menu = new dtkWidgetsMenu(fa::arrowcircleup, "Output Forms");
    dtkWidgetsMenuItem *add_output = this->output_menu->addItem(fa::plus,"Add output...");
    connect(add_output, &dtkWidgetsMenuItem::clicked,this, &gnomonPythonAlgorithmPluginEditor::addOutputForm);

    this->form_pane = new dtkWidgetsMenuBarContainer(this);
    this->form_pane->navigator->deleteLater();
    this->form_pane->build(QVector<dtkWidgetsMenu *>() << this->input_menu << this->output_menu);

    this->parameter_menu = new dtkWidgetsMenu(fa::gear, "Parameters");
    dtkWidgetsMenuItem *add_parameter = this->parameter_menu->addItem(fa::plus,"Add parameter...");
    QObject::connect(add_parameter, &dtkWidgetsMenuItem::clicked, [=] () {
        qDebug()<<"Add parameter";
    });

    this->parameter_pane = new dtkWidgetsMenuBarContainer(this);
    this->parameter_pane->navigator->deleteLater();
    this->parameter_pane->build(QVector<dtkWidgetsMenu *>() << this->parameter_menu);

    menu_layout->addWidget(this->form_pane);
    menu_layout->addWidget(this->parameter_pane);

    QWidget *menu_pane = new QWidget(this);
    menu_pane->setLayout(menu_layout);

    this->layout->insertWidget(0,menu_pane);

    this->setLayout(this->layout);
}

gnomonPythonAlgorithmPluginEditor::~gnomonPythonAlgorithmPluginEditor(void)
{
}

void gnomonPythonAlgorithmPluginEditor::updateDataPlugins(const QString& form_type)
{
    this->data_plugin_edit->clear();
    QList<QString> factory_keys;
    if (form_type == "gnomonCellComplex") {
        factory_keys = gnomonCore::cellComplexData::pluginFactory().keys();
    } else if (form_type == "gnomonCellImage") {
        factory_keys = gnomonCore::cellImageData::pluginFactory().keys();
    } else if (form_type == "gnomonImage") {
        factory_keys = gnomonCore::imageData::pluginFactory().keys();
    } else if (form_type == "gnomonMesh") {
        factory_keys = gnomonCore::meshData::pluginFactory().keys();
    } else if (form_type == "gnomonPointCloud") {
        factory_keys = gnomonCore::pointCloudData::pluginFactory().keys();
    }
    for (const auto& data_plugin : factory_keys) {
        this->data_plugin_edit->addItem(data_plugin);
    }
}

gnomonFormDescription *gnomonPythonAlgorithmPluginEditor::formDescriptionDialog(bool input)
{
    QDialog *form_dialog = new QDialog(this);
    form_dialog->setWindowModality(Qt::WindowModal);

    QGridLayout *form_layout = new QGridLayout();
    form_layout->setContentsMargins(10, 10, 10, 10);

    QString prefix = input? "Input" : "Output";
    QLabel *type_label = new QLabel(prefix+" form type");
    form_layout->addWidget(type_label, 0, 0, 1, 1);

    if (!this->type_edit) {
        this->type_edit = new QComboBox();

        type_edit->addItem("gnomonCellComplex");
        type_edit->addItem("gnomonCellImage");
        type_edit->addItem("gnomonImage");
        type_edit->addItem("gnomonMesh");
        type_edit->addItem("gnomonPointCloud");

        connect(this->type_edit, &QComboBox::currentTextChanged, this, &gnomonPythonAlgorithmPluginEditor::updateDataPlugins);
    }
    form_layout->addWidget(this->type_edit, 0, 1, 1, 1);

    QLabel *data_plugin_label = new QLabel("Form data plugin");
    form_layout->addWidget(data_plugin_label, 1, 0, 1, 1);

    if (!this->data_plugin_edit) {
        this->data_plugin_edit = new QComboBox();
    }
    this->updateDataPlugins(this->type_edit->currentText());
    form_layout->addWidget(this->data_plugin_edit, 1, 1, 1, 1);

    QLabel *name_label = new QLabel(prefix+" variable name");
    form_layout->addWidget(name_label, 2, 0, 1, 1);

    QLineEdit *name_edit = new QLineEdit();
    form_layout->addWidget(name_edit, 2, 1, 1, 1);

    QPushButton *cancel_button = new QPushButton("Cancel");
    cancel_button->setDefault(false);
    connect(cancel_button, &QPushButton::clicked, form_dialog, &QDialog::reject);
    form_layout->addWidget(cancel_button, 3, 0, 1, 1);

    QPushButton *ok_button = new QPushButton("Ok");
    ok_button->setDefault(true);
    connect(ok_button, &QPushButton::clicked, form_dialog, &QDialog::accept);
    form_layout->addWidget(ok_button, 3, 1, 1, 1);

    form_dialog->setLayout(form_layout);

    if (form_dialog->exec() & !name_edit->text().isEmpty())
    {
        return new gnomonFormDescription(name_edit->text(),this->type_edit->currentText(),this->data_plugin_edit->currentText());
    } else {
        return nullptr;
    }
}

void gnomonPythonAlgorithmPluginEditor::addInputForm()
{
    gnomonFormDescription *form_description = this->formDescriptionDialog(true);
    if (form_description) {
        qDebug()<<form_description->name<<form_description->type;
        this->input_forms.append(form_description);
    }
}

void gnomonPythonAlgorithmPluginEditor::addOutputForm()
{
    gnomonFormDescription *form_description = this->formDescriptionDialog(false);
    if (form_description) {
        qDebug()<<form_description->name<<form_description->type;
        this->output_forms.append(form_description);
    }
}


// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePythonAlgorithmPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonWorkspacePythonAlgorithmPrivate
{
public:
    dtkWidgetsMenu *menu(dtkWidgetsWorkspace *);
    void configure(void);

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
    QVBoxLayout *viewer_layout = nullptr;

public:
    QHash<QString, dtkCoreParameter *> parameters;

public:
    gnomonAbstractFormAlgorithm *algorithm = nullptr;
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
            this->editor->editor->setText(s.readAll());

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
    QString output = dtkScriptInterpreterPython::instance()->interpret(this->editor->editor->toPlainText(), &stat);

    if (gnomonCore::formAlgorithm::pluginFactory().keys().size() > 0) {
        QString key = gnomonCore::formAlgorithm::pluginFactory().keys()[0];
        qDebug()<<Q_FUNC_INFO<<key;
        this->algorithm = gnomonCore::formAlgorithm::pluginFactory().create(key);
        Q_ASSERT(this->algorithm);
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

        QMap<QString, gnomonCoreParameter *> parameters = this->algorithm->parameters();

        for(QMap<QString, gnomonCoreParameter*>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it) {
            QWidget *widget = gnomonWidgetsParameter::widget(it.value(), 0);
            if (widget)
                this->layout->addRow(it.key(), widget);
        }

        this->layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
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

    QString example = "import gnomoncore\n";
    example += "\n";
    example += "from gnomon_utils import gnomonPlugin, gnomonParametric\n";
    example += "\n";
    example += "@gnomonPlugin(namespace=gnomoncore)\n";
    example += "@gnomonParametric\n";
    example += "class pythonAlgorithm(gnomoncore.gnomonAbstractFormAlgorithm):\n";
    example += "\n";
    example += "    def __init__(self):\n";
    example += "        super().__init__()\n";
    example += "\n";
    example += "        self._parameters = {}\n";
    example += "\n";
    example += "    def run(self):\n";
    example += "        pass\n";

    d->editor->editor->setText(example);

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

    QSplitter *editor_splitter = new QSplitter(this);
    editor_splitter->setOrientation(Qt::Vertical);
    editor_splitter->addWidget(d->editor);
    editor_splitter->addWidget(d->terminal);

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

//    QSplitter *viewer_splitter = new QSplitter(this);
//    viewer_splitter->setOrientation(Qt::Vertical);
//    viewer_splitter->addWidget(d->source);
//    viewer_splitter->addWidget(d->target_stack);

    d->viewer_layout = new QVBoxLayout;
    d->viewer_layout->setContentsMargins(0, 0, 0, 0);
    d->viewer_layout->setSpacing(0);
    d->viewer_layout->addWidget(d->source);
    d->viewer_layout->addWidget(d->target_stack);

    QWidget *viewer = new QWidget(this);
    viewer->setLayout(d->viewer_layout);

//     // -- Organizing the whole workspace --
    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    splitter->addWidget(editor_splitter);
    splitter->addWidget(viewer);


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(splitter);
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
    Q_ASSERT(d->algorithm);

    if (d->source->cellComplex()) {
        d->algorithm->setInputCellComplex(d->source->cellComplex());
    }
    if (d->source->cellImage()) {
        d->algorithm->setInputCellImage(d->source->cellImage());
    }
    if (d->source->image()) {
        d->algorithm->setInputImage(d->source->image());
    }
    if (d->source->mesh()) {
        d->algorithm->setInputMesh(d->source->mesh());
    }
    if (d->source->pointCloud()) {
        d->algorithm->setInputPointCloud(d->source->pointCloud());
    }

    d->algorithm->run();

    d->target_stack->setCurrentWidget(d->target_message);
    d->source->setEnableLinking(false);
    d->target->setEnableLinking(false);


    gnomonCellComplexSeries *cellComplex = d->algorithm->outputCellComplex();
    if ((!cellComplex)||(cellComplex->times().size()==0)) {
        qDebug()<<"No CellComplex!";
    } else {
        d->target->setForm("gnomonCellComplex",cellComplex);
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);
    }

    gnomonCellImageSeries *cellImage = d->algorithm->outputCellImage();
    if ((!cellImage)||(cellImage->times().size()==0)) {
        qDebug()<<"No CellImage!";
    } else {
        d->target->setForm("gnomonCellImage",cellImage);
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);
    }

    gnomonImageSeries *image = d->algorithm->outputImage();
     if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        qDebug()<<"No Image!";
    } else {
        d->target->setForm("gnomonImage",image);
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);
    }

    gnomonMeshSeries *mesh = d->algorithm->outputMesh();
    if ((!mesh)||(mesh->times().size()==0)) {
        qDebug()<<"No Mesh!";
    } else {
        d->target->setForm("gnomonMesh",mesh);
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);
    }

    gnomonPointCloudSeries *pointCloud = d->algorithm->outputPointCloud();
    if ((!pointCloud)||(pointCloud->times().size()==0)) {
        qDebug()<<"No PointCloud!";
    } else {
        d->target->setForm("gnomonPointCloud",pointCloud);
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);
    }
}

const QColor gnomonWorkspacePythonAlgorithm::color = QColor("#a38948");

bool gnomonWorkspacePythonAlgorithm::isEmpty(void)
{
    return false;
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonWorkspacePythonAlgorithm.moc"


//
// gnomonWorkspacePythonAlgorithm.cpp ends here
