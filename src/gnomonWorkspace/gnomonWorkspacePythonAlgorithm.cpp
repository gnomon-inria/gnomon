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
// gnomonParameterDescription
// ///////////////////////////////////////////////////////////////////

class gnomonParameterDescription
{
public:
     gnomonParameterDescription(const QString& name, const QString& type, const QString& doc);
    ~gnomonParameterDescription(void);

public:
    QString name;
    QString type;
    QString doc;
};

gnomonParameterDescription::gnomonParameterDescription(const QString& name, const QString& type, const QString& doc)
{
    this->name = name;
    this->type = type;
    this->doc = doc;
}

gnomonParameterDescription::~gnomonParameterDescription(void)
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
    QHBoxLayout *layout = nullptr;
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
    this->editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QFont font = this->editor->font();
    font.setFamily("Courier New");
    this->editor->setCurrentFont(font);

    // -- Organizing the editor column --
    this->layout = new QHBoxLayout;
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
    dtkWidgetsMenu *newFormMenu(bool input = true);
    dtkWidgetsMenu *newParameterMenu(void);

public slots:
    void updateDataPlugins(const QString& form_type);
    void addInputForm(gnomonFormDescription *desc);
    void addOutputForm(gnomonFormDescription *desc);
    void addParameter(gnomonParameterDescription* desc);

    void updateMenus(void);
    void updateCode(void);

public:
    QMap<QString, gnomonFormDescription *> input_forms;
    QMap<QString, gnomonFormDescription *> output_forms;
    QMap<QString, gnomonParameterDescription *> parameters;

public:
    QVBoxLayout *menu_layout = nullptr;

    dtkWidgetsMenuBarContainer *form_pane = nullptr;
    dtkWidgetsMenu *input_menu = nullptr;
    dtkWidgetsMenu *add_input = nullptr;

    dtkWidgetsMenu *output_menu = nullptr;
    dtkWidgetsMenu *add_output = nullptr;

    dtkWidgetsMenuBarContainer *parameter_pane = nullptr;
    dtkWidgetsMenu *parameter_menu = nullptr;
    dtkWidgetsMenu *add_parameter = nullptr;

public:
    QComboBox *type_edit = nullptr;
    QComboBox *data_plugin_edit = nullptr;
};

gnomonPythonAlgorithmPluginEditor::gnomonPythonAlgorithmPluginEditor(QWidget *parent) : gnomonPythonScriptEditor(parent)
{
    this->menu_layout = new QVBoxLayout();
    this->menu_layout->setContentsMargins(0, 0, 0, 0);

//    this->input_menu = new dtkWidgetsMenu(fa::arrowcircledown, "Input Forms");
//    this->add_input = this->input_menu->addItem(fa::plus,"Add input...");
//    connect(this->add_input, &dtkWidgetsMenuItem::clicked, this, &gnomonPythonAlgorithmPluginEditor::addInputForm);
//
//    this->output_menu = new dtkWidgetsMenu(fa::arrowcircleup, "Output Forms");
//    this->add_output = this->output_menu->addItem(fa::plus,"Add output...");
//    connect(this->add_output, &dtkWidgetsMenuItem::clicked, this, &gnomonPythonAlgorithmPluginEditor::addOutputForm);
//
//    this->form_pane = new dtkWidgetsMenuBarContainer(this);
//    this->form_pane->navigator->setVisible(false);
//    this->form_pane->build(QVector<dtkWidgetsMenu *>() << this->input_menu << this->output_menu);
//    this->form_pane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->updateMenus();

    this->menu_layout->addWidget(this->form_pane);
    this->menu_layout->addWidget(this->parameter_pane);

    QWidget *menu_pane = new QWidget(this);
    menu_pane->setLayout(menu_layout);
    menu_pane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    menu_pane->setFixedWidth(300);

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

dtkWidgetsMenu *gnomonPythonAlgorithmPluginEditor::newFormMenu(bool input)
{
    QGridLayout *form_layout = new QGridLayout();
    form_layout->setContentsMargins(10, 10, 10, 10);

    QString prefix = input? "Input" : "Output";
    QLabel *type_label = new QLabel(prefix+" form type");
    form_layout->addWidget(type_label, 0, 0, 1, 1);

    QComboBox *type_edit = new QComboBox();
    type_edit->addItem("gnomonCellComplex");
    type_edit->addItem("gnomonCellImage");
    type_edit->addItem("gnomonImage");
    type_edit->addItem("gnomonMesh");
    type_edit->addItem("gnomonPointCloud");
    form_layout->addWidget(type_edit, 0, 1, 1, 1);

    QLabel *data_plugin_label = new QLabel("Form data plugin");
    form_layout->addWidget(data_plugin_label, 1, 0, 1, 1);

    QComboBox *data_plugin_edit = new QComboBox();
    form_layout->addWidget(data_plugin_edit, 1, 1, 1, 1);
    
    connect(type_edit, &QComboBox::currentTextChanged, [=] (const QString& form_type) {
        data_plugin_edit->clear();
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
            data_plugin_edit->addItem(data_plugin);
        }
    });
    type_edit->setCurrentText("gnomonImage");

    QLabel *name_label = new QLabel(prefix+" variable name");
    form_layout->addWidget(name_label, 2, 0, 1, 1);

    QLineEdit *name_edit = new QLineEdit();
    form_layout->addWidget(name_edit, 2, 1, 1, 1);

    static std::function<void (void)> ca = [=] (void) -> void
    {
        this->form_pane->slider->blockSignals(false);
        this->form_pane->slider->enableSpying(true);
    };

    static std::function<void (void)> cb = [=] (void) -> void
    {
        this->form_pane->slider->setCurrentIndex(0,ca);
        this->form_pane->decr();
    };

    QPushButton *cancel_button = new QPushButton("Cancel");
    cancel_button->setDefault(false);
    connect(cancel_button, &QPushButton::clicked, [=] () {
        this->form_pane->switchToRoot(cb);
    });
    form_layout->addWidget(cancel_button, 3, 0, 1, 1);

    QPushButton *ok_button = new QPushButton("Ok");
    ok_button->setDefault(true);
    connect(ok_button, &QPushButton::clicked, [=] () {
        this->form_pane->switchToRoot(cb);
        if (!name_edit->text().isEmpty()) {
            gnomonFormDescription *desc = new gnomonFormDescription(name_edit->text(),type_edit->currentText(),data_plugin_edit->currentText());
            if (input) {
                this->addInputForm(desc);
            } else {
                this->addOutputForm(desc);
            }
        }
    });
    form_layout->addWidget(ok_button, 3, 1, 1, 1);

    QWidget *form_widget = new QWidget();
    form_widget->setLayout(form_layout);

    dtkWidgetsMenuItemDIY *new_form_item = new dtkWidgetsMenuItemDIY("New " + prefix + " Form");
    new_form_item->addWidget(form_widget);

    dtkWidgetsMenu *new_form_menu = new dtkWidgetsMenu(fa::plus, "Add form...");
    new_form_menu->addItem(new_form_item);

    return new_form_menu;
}

dtkWidgetsMenu *gnomonPythonAlgorithmPluginEditor::newParameterMenu(void)
{
    QGridLayout *parameter_layout = new QGridLayout();
    parameter_layout->setContentsMargins(10, 10, 10, 10);

    QLabel *name_label = new QLabel("Parameter name");
    parameter_layout->addWidget(name_label, 0, 0, 1, 1);

    QLineEdit *name_edit = new QLineEdit();
    parameter_layout->addWidget(name_edit, 0, 1, 1, 1);

    QLabel *type_label = new QLabel("Parameter type");
    parameter_layout->addWidget(type_label, 1, 0, 1, 1);

    QComboBox *type_edit = new QComboBox();
    type_edit->addItem("Bool");
    type_edit->addItem("Int");
    type_edit->addItem("Double");
    type_edit->addItem("String");
    type_edit->addItem("StringList");
    parameter_layout->addWidget(type_edit, 1, 1, 1, 1);

    QLabel *documentation_label = new QLabel("Documentation");
    parameter_layout->addWidget(documentation_label, 2, 0, 1, 1);

    QLineEdit *documentation_edit = new QLineEdit();
    parameter_layout->addWidget(documentation_edit, 2, 1, 1, 1);

    static std::function<void (void)> ca = [=] (void) -> void
    {
        this->parameter_pane->slider->blockSignals(false);
        this->parameter_pane->slider->enableSpying(true);
    };

    static std::function<void (void)> cb = [=] (void) -> void
    {
        this->parameter_pane->slider->setCurrentIndex(0,ca);
        this->parameter_pane->decr();
    };

    QPushButton *cancel_button = new QPushButton("Cancel");
    cancel_button->setDefault(false);
    connect(cancel_button, &QPushButton::clicked, [=] (){
        this->parameter_pane->switchToRoot(cb);
    });
    parameter_layout->addWidget(cancel_button, 3, 0, 1, 1);

    QPushButton *ok_button = new QPushButton("Ok");
    ok_button->setDefault(true);
    connect(ok_button, &QPushButton::clicked, [=] (){
        this->parameter_pane->switchToRoot(cb);
        if (!name_edit->text().isEmpty()) {
            gnomonParameterDescription *desc = new gnomonParameterDescription(name_edit->text(),type_edit->currentText(),documentation_edit->text());
            this->addParameter(desc);
        }
    });
    parameter_layout->addWidget(ok_button, 3, 1, 1, 1);

    QWidget *parameter_widget = new QWidget();
    parameter_widget->setLayout(parameter_layout);

    dtkWidgetsMenuItemDIY *new_parameter_item = new dtkWidgetsMenuItemDIY("New Parameter");
    new_parameter_item->addWidget(parameter_widget);

    dtkWidgetsMenu *new_parameter_menu = new dtkWidgetsMenu(fa::plus, "Add parameter...");
    new_parameter_menu->addItem(new_parameter_item);

    return new_parameter_menu;
}

void gnomonPythonAlgorithmPluginEditor::addInputForm(gnomonFormDescription *desc)
{
    if (desc) {
        this->input_forms[desc->type] = desc;
        this->updateMenus();
        this->updateCode();
    }
}

void gnomonPythonAlgorithmPluginEditor::addOutputForm(gnomonFormDescription *desc)
{
    if (desc) {
        this->output_forms[desc->type] = desc;
        this->updateMenus();
        this->updateCode();
    }
}

void gnomonPythonAlgorithmPluginEditor::addParameter(gnomonParameterDescription *desc)
{
    if (desc) {
        this->parameters[desc->name] = desc;
        this->updateMenus();
        this->updateCode();
    }
}

void gnomonPythonAlgorithmPluginEditor::updateMenus(void)
{
    if (!this->input_menu) {
        this->input_menu = new dtkWidgetsMenu(fa::arrowcircledown, "Input Forms");
    }
    
    if (!this->output_menu) {
        this->output_menu = new dtkWidgetsMenu(fa::arrowcircleup, "Output Forms");
    }
    
    if (!this->form_pane) {
        this->form_pane = new dtkWidgetsMenuBarContainer(this);
        this->form_pane->navigator->setVisible(false);
        this->form_pane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        this->form_pane->q = this->script_menubar;
    }
    
    if (this->add_input) {
        this->form_pane->slides.remove(this->add_input);
        this->input_menu->removeMenu(this->add_input);
        delete this->add_input;
    }
    
    for (const auto &form_type : this->input_forms.keys()) {
        gnomonFormDescription *desc = this->input_forms[form_type];
        QString short_type = desc->type.split("gnomon")[1];
        short_type.replace(0,1,short_type[0].toUpper());
        dtkWidgetsMenuItem *input_item = new dtkWidgetsMenuItem(fa::image, desc->name + " (" + short_type + ")");
        this->input_menu->addItem(input_item);
    }
    
    this->add_input = this->input_menu->addMenu(this->newFormMenu(true));
    
    if (this->add_output) {
        this->form_pane->slides.remove(this->add_output);
        this->output_menu->removeMenu(this->add_output);
        delete this->add_output;
    }
    
    for (const auto &form_type : this->output_forms.keys()) {
        gnomonFormDescription *desc = this->output_forms[form_type];
        QString short_type = desc->type.split("gnomon")[1];
        short_type.replace(0,1,short_type[0].toUpper());
        dtkWidgetsMenuItem *output_item = new dtkWidgetsMenuItem(fa::image, desc->name + " (" + short_type + ")");
        this->output_menu->addItem(output_item);
    }
    
    this->add_output = this->output_menu->addMenu(this->newFormMenu(false));
    
    this->form_pane->build(QVector<dtkWidgetsMenu *>() << this->input_menu << this->output_menu);
    this->form_pane->buildChildSlide(this->add_input);
    this->form_pane->buildChildSlide(this->add_output);
    this->form_pane->touch();


    if (!this->parameter_menu) {
        this->parameter_menu = new dtkWidgetsMenu(fa::gear, "Parameters");
    }

    if (!this->parameter_pane) {
        this->parameter_pane = new dtkWidgetsMenuBarContainer(this);
        this->parameter_pane->navigator->setVisible(false);
        this->parameter_pane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        this->parameter_pane->q = this->script_menubar;
    }

    if (this->add_parameter) {
        this->parameter_pane->slides.remove(this->add_parameter);
        this->parameter_menu->removeMenu(this->add_parameter);
        delete this->add_parameter;
    }

    for (const auto &param : this->parameters.keys()) {
        qDebug()<<param<<this->parameters[param];
        gnomonParameterDescription *desc = this->parameters[param];
        dtkWidgetsMenuItem *parameter_item = new dtkWidgetsMenuItem(fa::circlethin, desc->name + " (" + desc->type + ")");
        this->parameter_menu->addItem(parameter_item);
    }

    this->add_parameter = this->parameter_menu->addMenu(this->newParameterMenu());

    this->parameter_pane->build(QVector<dtkWidgetsMenu *>() << this->parameter_menu);
    this->parameter_pane->buildChildSlide(this->add_parameter);
    this->parameter_pane->touch();

    this->menu_layout->addWidget(this->parameter_pane);
}

void gnomonPythonAlgorithmPluginEditor::updateCode(void)
{
    QString current_code = this->editor->toPlainText();

    QStringList code_lines = current_code.split("\n");
    QString import_code = "";
    QString run_code = "";

    bool user_line = false;
    int section = 0;
    for (const auto& line : code_lines) {

        if (line.contains("# {#")) {
            user_line = false;
            section++;
        }
        if (user_line) {
            if (section == 1) {
                import_code += line + "\n";
            } else {
                run_code += line + "\n";
            }
        }
        if (line.contains("# #}")) {
            user_line = true;
        }
    }
    run_code = run_code.left(run_code.length()-1);

    QString plugin_code = "";

    plugin_code += "# {# gnomon, plugin.imports\n";

    plugin_code += "import gnomoncore\n";
    plugin_code += "\n";
    plugin_code += "from gnomon_utils import gnomonPlugin, gnomonParametric\n";

    if (this->input_forms.size() + this->output_forms.size() > 0) {
        plugin_code += "from gnomon_utils.gnomonDecorator import";
    }
    int n_forms = 0;
    for (const auto &form_type : this->input_forms.keys()) {
        if (n_forms > 0) {
            plugin_code += ",";
        }
        plugin_code += " " + form_type + "Input";
        n_forms ++;
    }
    for (const auto &form_type : this->output_forms.keys()) {
        if (n_forms > 0) {
            plugin_code += ",";
        }
        plugin_code += " " + form_type + "Output";
        n_forms ++;
    }
    plugin_code += "\n";

    plugin_code += "# #}\n";

    if (import_code.isEmpty()) {
        plugin_code += "# add your imports before the next gnomon tag\n";

        plugin_code += "\n";
        plugin_code += "\n";
    } else {
        plugin_code += import_code;
    }

    plugin_code += "# {# gnomon, plugin.class\n";

    plugin_code += "@gnomonPlugin(namespace=gnomoncore)\n";
    plugin_code += "@gnomonParametric\n";

    for (const auto &form_type : this->input_forms.keys()) {
        gnomonFormDescription *desc = this->input_forms[form_type];
        plugin_code += "@" + form_type + "Input(";
        plugin_code += "attr='" + desc->name + "', ";
        plugin_code += "method='input" + desc->type.split("gnomon")[1] + "', ";
        plugin_code += "setter_method='setInput" + desc->type.split("gnomon")[1] + "', ";
        plugin_code += "data_plugin='" + desc->data_plugin + "')\n";
    }
    for (const auto &form_type : this->output_forms.keys()) {
        gnomonFormDescription *desc = this->output_forms[form_type];
        plugin_code += "@" + form_type + "Output(";
        plugin_code += "attr='" + desc->name + "', ";
        plugin_code += "method='output" + desc->type.split("gnomon")[1] + "', ";
        plugin_code += "data_plugin='" + desc->data_plugin + "')\n";
    }
    plugin_code += "class pythonAlgorithm(gnomoncore.gnomonAbstractFormAlgorithm):\n";
    plugin_code += "\n";
    plugin_code += "    def __init__(self):\n";
    plugin_code += "        super().__init__()\n";
    plugin_code += "\n";
    plugin_code += "        self._parameters = {}\n";
    for (const auto &param : this->parameters.keys()) {
        gnomonParameterDescription *desc = this->parameters[param];
        plugin_code += "        self._parameters['" + desc->name + "'] = ";
        plugin_code += "gnomoncore.Parameter" + desc->type + "(";
        if (desc->type == "Bool") {
            plugin_code += "True";
        } else if (desc->type == "Int") {
            plugin_code += "1,0,10";
        } else if (desc->type == "Double") {
            plugin_code += "1.,0.,1.,2";
        } else if (desc->type == "String") {
            plugin_code += "'',['']";
        } else if (desc->type == "StringList") {
            plugin_code += "[''],['']";
        }
        plugin_code += ",'" + desc->doc + "')\n";
    }
    plugin_code += "\n";
    if (n_forms > 0) {
        for (const auto &form_type : this->input_forms.keys()) {
            gnomonFormDescription *desc = this->input_forms[form_type];
            plugin_code += "        self." + desc->name + " = {}\n";
        }
        for (const auto &form_type : this->output_forms.keys()) {
            gnomonFormDescription *desc = this->output_forms[form_type];
            plugin_code += "        self." + desc->name + " = {}\n";
        }
        plugin_code += "\n";
    }
    plugin_code += "    def run(self):\n";
    if (n_forms == 0)
    {
        plugin_code += "        # #}\n";
        if (run_code.isEmpty() | !run_code.startsWith("        ")) {
            plugin_code += "        # implement the run method\n";
            plugin_code += "\n";
            plugin_code += "        pass\n";
        } else {
            plugin_code += run_code;
        }
    } else {
        for (const auto &form_type : this->output_forms.keys()) {
            gnomonFormDescription *desc = this->output_forms[form_type];
            plugin_code += "        self." + desc->name + " = {}\n";
        }

        if (this->input_forms.size()>0) {
            plugin_code += "        for time in self." + this->input_forms.values()[0]->name +  ".keys():\n";
        } else {
            plugin_code += "        for time in [0]:\n";
        }
        for (const auto &form_type : this->input_forms.keys()) {
            gnomonFormDescription *desc = this->input_forms[form_type];
            plugin_code += "            " + desc->name + " = self." + desc->name + "[time]\n";
        }
        plugin_code += "            # #}\n";
        if (run_code.isEmpty() | !run_code.startsWith("            ")) {
            plugin_code += "            # implement the run method\n";
            plugin_code += "\n";
            for (const auto &form_type : this->output_forms.keys()) {
                gnomonFormDescription *desc = this->output_forms[form_type];
                plugin_code += "            self." + desc->name + "[time] = None\n";
            }
        } else {
            plugin_code += run_code;
        }
    }

    this->editor->setText(plugin_code);
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
    QHBoxLayout *viewer_layout = nullptr;

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

//        this->layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
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

    if (d->algorithm) {

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
