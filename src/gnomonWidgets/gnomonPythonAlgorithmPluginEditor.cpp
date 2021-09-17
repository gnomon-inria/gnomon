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

#include "gnomonPythonAlgorithmPluginEditor.h"

#include <gnomonCore>

#include <dtkCore>
#include <dtkScript>
#include <dtkFonts>
#include <dtkMacs>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>


class gnomonPythonAlgorithmPluginEditorPrivate
{
public:
    QMap<QString, QString> parameter_types;

public:
    QMap<QString, gnomonFormDescription> input_forms;
    QMap<QString, gnomonFormDescription> output_forms;
    QMap<QString, gnomonParameterDescription> parameters;

public:
    gnomonPythonPluginParser *parser;

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

public:
     gnomonPythonAlgorithmPluginEditorPrivate(void);
    ~gnomonPythonAlgorithmPluginEditorPrivate(void);
};

gnomonPythonAlgorithmPluginEditorPrivate::gnomonPythonAlgorithmPluginEditorPrivate(void)
{
    this->parser = new gnomonPythonPluginParser();

    this->parameter_types = this->parser->parameterTypes();
}


gnomonPythonAlgorithmPluginEditorPrivate::~gnomonPythonAlgorithmPluginEditorPrivate(void)
{
}

// ///////////////////////////////////////////////////////////////////
// gnomonPythonAlgorithmPluginEditor
// ///////////////////////////////////////////////////////////////////


gnomonPythonAlgorithmPluginEditor::gnomonPythonAlgorithmPluginEditor(QWidget *parent) : gnomonPythonEditor(parent)
{
    dd = new gnomonPythonAlgorithmPluginEditorPrivate();

    dd->menu_layout = new QVBoxLayout();
    dd->menu_layout->setContentsMargins(0, 0, 0, 0);

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

//    this->menu_layout->addWidget(this->form_pane);
//    this->menu_layout->addWidget(this->parameter_pane);

    QWidget *menu_pane = new QWidget(this);
    menu_pane->setLayout(dd->menu_layout);
    menu_pane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    menu_pane->setFixedWidth(300);

    d->layout->insertWidget(0,menu_pane);
//
//    this->setLayout(this->layout);
}

gnomonPythonAlgorithmPluginEditor::~gnomonPythonAlgorithmPluginEditor(void)
{
    delete dd;
}

const QMap<QString, gnomonFormDescription>& gnomonPythonAlgorithmPluginEditor::inputForms(void) const
{
    return dd->input_forms;
}

const QMap<QString, gnomonFormDescription>& gnomonPythonAlgorithmPluginEditor::outputForms(void) const
{
    return dd->output_forms;
}

const QMap<QString, gnomonParameterDescription>& gnomonPythonAlgorithmPluginEditor::parameters(void) const
{
    return dd->parameters;
}

void gnomonPythonAlgorithmPluginEditor::updateDataPlugins(const QString& form_type)
{
    dd->data_plugin_edit->clear();
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
        dd->data_plugin_edit->addItem(data_plugin);
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
    type_edit->setCurrentIndex(1);
    type_edit->setCurrentIndex(0);

    QLabel *name_label = new QLabel(prefix+" variable name");
    form_layout->addWidget(name_label, 2, 0, 1, 1);

    QLineEdit *name_edit = new QLineEdit();
    form_layout->addWidget(name_edit, 2, 1, 1, 1);

    static std::function<void (void)> ca = [=] (void) -> void
    {
        dd->form_pane->slider->blockSignals(false);
        dd->form_pane->slider->enableSpying(true);
    };

    static std::function<void (void)> cb = [=] (void) -> void
    {
        dd->form_pane->slider->setCurrentIndex(0,ca);
    };

    QPushButton *cancel_button = new QPushButton("Cancel");
    cancel_button->setDefault(false);
    connect(cancel_button, &QPushButton::clicked, [=] () {
        dd->form_pane->switchToRoot(cb);
        this->updateMenus();
    });
    form_layout->addWidget(cancel_button, 3, 0, 1, 1);

    QPushButton *ok_button = new QPushButton("Ok");
    ok_button->setDefault(true);
    connect(ok_button, &QPushButton::clicked, [=] () {
        dd->form_pane->switchToRoot(cb);
        if (!name_edit->text().isEmpty()) {
            gnomonFormDescription desc(name_edit->text(),type_edit->currentText(),data_plugin_edit->currentText());
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
    for (const auto& type : dd->parameter_types.keys()) {
        type_edit->addItem(type);
    }
    parameter_layout->addWidget(type_edit, 1, 1, 1, 1);

    QLabel *documentation_label = new QLabel("Documentation");
    parameter_layout->addWidget(documentation_label, 2, 0, 1, 1);

    QLineEdit *documentation_edit = new QLineEdit();
    parameter_layout->addWidget(documentation_edit, 2, 1, 1, 1);

    static std::function<void (void)> ca = [=] (void) -> void
    {
        dd->parameter_pane->slider->blockSignals(false);
        dd->parameter_pane->slider->enableSpying(true);
    };

    static std::function<void (void)> cb = [=] (void) -> void
    {
        dd->parameter_pane->slider->setCurrentIndex(0,ca);
    };

    QPushButton *cancel_button = new QPushButton("Cancel");
    cancel_button->setDefault(false);
    connect(cancel_button, &QPushButton::clicked, [=] (){
        dd->parameter_pane->switchToRoot(cb);
        this->updateMenus();
    });
    parameter_layout->addWidget(cancel_button, 3, 0, 1, 1);

    QPushButton *ok_button = new QPushButton("Ok");
    ok_button->setDefault(true);
    connect(ok_button, &QPushButton::clicked, [=] (){
        dd->parameter_pane->switchToRoot(cb);
        if (!name_edit->text().isEmpty()) {
            gnomonParameterDescription desc(name_edit->text(),type_edit->currentText(),documentation_edit->text());
            this->addParameter(desc);
        }
    });
    parameter_layout->addWidget(ok_button, 3, 1, 1, 1);

    QWidget *parameter_widget = new QWidget();
    parameter_widget->setLayout(parameter_layout);

    dtkWidgetsMenuItemDIY *new_parameter_item = new dtkWidgetsMenuItemDIY("New Parameter");
    new_parameter_item->addWidget(parameter_widget);

    if (!dd->add_parameter) {
        dd->add_parameter = new dtkWidgetsMenu(fa::plus, "Add parameter...");
    }
    dd->add_parameter->addItem(new_parameter_item);

    return dd->add_parameter;
}

void gnomonPythonAlgorithmPluginEditor::addInputForm(gnomonFormDescription desc, bool update_code)
{
    dd->input_forms[desc.type] = desc;
    this->updateMenus();
    if (update_code) {
        this->updateCode();
    }
}

void gnomonPythonAlgorithmPluginEditor::addOutputForm(gnomonFormDescription desc, bool update_code)
{
    dd->output_forms[desc.type] = desc;
    this->updateMenus();
    if (update_code) {
        this->updateCode();
    }
}

void gnomonPythonAlgorithmPluginEditor::addParameter(gnomonParameterDescription desc, bool update_code)
{
    dd->parameters[desc.name] = desc;
    this->updateMenus();
    if (update_code) {
        this->updateCode();
    }
}

void gnomonPythonAlgorithmPluginEditor::updateMenus(void)
{
    if (dd->form_pane) {
        dd->form_pane->disconnect();
        dd->form_pane->deleteLater();
        dd->form_pane = nullptr;
    }

    if (dd->input_menu) {
        dd->input_menu->disconnect();
        dd->input_menu->deleteLater();
        dd->input_menu = nullptr;
    }

    if (!dd->input_menu) {
        dd->input_menu = new dtkWidgetsMenu(fa::arrowcircledown, "Input Forms");
    }

    if (dd->output_menu) {
        dd->output_menu->disconnect();
        dd->output_menu->deleteLater();
        dd->output_menu = nullptr;
    }

    if (!dd->output_menu) {
        dd->output_menu = new dtkWidgetsMenu(fa::arrowcircleup, "Output Forms");
    }

    if (!dd->form_pane) {
        dd->form_pane = new dtkWidgetsMenuBarContainer(this);
        dd->form_pane->navigator->setVisible(false);
        dd->form_pane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        dd->form_pane->q = d->script_menubar;
    }

    if (dd->add_input) {
        dd->add_input->disconnect();
        dd->add_input->deleteLater();
        dd->add_input = nullptr;
    }

    for (const auto &form_type : dd->input_forms.keys()) {
        gnomonFormDescription desc = dd->input_forms[form_type];
        QString short_type = desc.type.split("gnomon")[1];
        short_type.replace(0,1,short_type[0].toUpper());
        dtkWidgetsMenuItem *input_item = new dtkWidgetsMenuItem(fa::image, desc.name + " (" + short_type + ")");
        dd->input_menu->addItem(input_item);
    }

    dd->add_input = dd->input_menu->addMenu(this->newFormMenu(true));

    if (dd->add_output) {
        dd->add_output->disconnect();
        dd->add_output->deleteLater();
        dd->add_output = nullptr;
    }

    for (const auto &form_type : dd->output_forms.keys()) {
        gnomonFormDescription desc = dd->output_forms[form_type];
        QString short_type = desc.type.split("gnomon")[1];
        short_type.replace(0,1,short_type[0].toUpper());
        dtkWidgetsMenuItem *output_item = new dtkWidgetsMenuItem(fa::image, desc.name + " (" + short_type + ")");
        dd->output_menu->addItem(output_item);
    }

    if (!dd->add_output) {
        dd->add_output = this->newFormMenu(false);
        dd->output_menu->addMenu(dd->add_output);
    }

    dd->form_pane->build(QVector<dtkWidgetsMenu *>() << dd->input_menu << dd->output_menu);
    dd->form_pane->buildChildSlide(dd->add_input);
    dd->form_pane->buildChildSlide(dd->add_output);

    dd->menu_layout->addWidget(dd->form_pane);

    if (dd->parameter_menu) {
        dd->parameter_menu->disconnect();
        dd->parameter_menu->deleteLater();
        dd->parameter_menu = nullptr;
    }

    if (!dd->parameter_menu) {
        dd->parameter_menu = new dtkWidgetsMenu(fa::gear, "Parameters");
    }

    if (dd->parameter_pane) {
        dd->parameter_pane->disconnect();
        dd->parameter_pane->deleteLater();
        dd->parameter_pane = nullptr;
    }

    if (!dd->parameter_pane) {
        dd->parameter_pane = new dtkWidgetsMenuBarContainer(this);
        dd->parameter_pane->navigator->setVisible(false);
        dd->parameter_pane->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        dd->parameter_pane->q = d->script_menubar;
    }

    if (dd->add_parameter) {
        dd->add_parameter->disconnect();
        dd->add_parameter->deleteLater();
        dd->add_parameter = nullptr;
    }

    for (const auto &param : dd->parameters.keys()) {
        gnomonParameterDescription desc = dd->parameters[param];
        dtkWidgetsMenuItem *parameter_item = new dtkWidgetsMenuItem(fa::circlethin, desc.name + " (" + desc.type + ")");
        dd->parameter_menu->addItem(parameter_item);
    }

    if (!dd->add_parameter) {
        dd->add_parameter = this->newParameterMenu();
        dd->parameter_menu->addMenu(dd->add_parameter);
    }
    dd->parameter_pane->build(QVector<dtkWidgetsMenu *>() << dd->parameter_menu);
    dd->parameter_pane->buildChildSlide(dd->add_parameter);

    dd->menu_layout->addWidget(dd->parameter_pane);
}

void gnomonPythonAlgorithmPluginEditor::updateCode(void)
{
    QString current_code = d->editor->toPlainText();

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

    plugin_code += "from dtkcore import d_bool, d_int, d_real, d_inliststring, d_inliststringlist\n";
    plugin_code += "\n";
    plugin_code += "import gnomoncore\n";
    plugin_code += "\n";
    plugin_code += "from gnomon_utils import gnomonPlugin, gnomonParametric\n";

    if (dd->input_forms.size() + dd->output_forms.size() > 0) {
        plugin_code += "from gnomon_utils.gnomonDecorator import";
    }
    int n_forms = 0;
    for (const auto &form_type : dd->input_forms.keys()) {
        if (n_forms > 0) {
            plugin_code += ",";
        }
        plugin_code += " " + form_type + "Input";
        n_forms ++;
    }
    for (const auto &form_type : dd->output_forms.keys()) {
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

    plugin_code += "@gnomonPlugin(version='0.1.0', coreversion='0.17.0', namespace=gnomoncore)\n";
    plugin_code += "@gnomonParametric\n";

    for (const auto &form_type : dd->input_forms.keys()) {
        gnomonFormDescription desc = dd->input_forms[form_type];
        plugin_code += "@" + form_type + "Input(";
        plugin_code += "attr='" + desc.name + "', ";
        plugin_code += "method='input" + desc.type.split("gnomon")[1] + "', ";
        plugin_code += "setter_method='setInput" + desc.type.split("gnomon")[1] + "', ";
        plugin_code += "data_plugin='" + desc.data_plugin + "')\n";
    }
    for (const auto &form_type : dd->output_forms.keys()) {
        gnomonFormDescription desc = dd->output_forms[form_type];
        plugin_code += "@" + form_type + "Output(";
        plugin_code += "attr='" + desc.name + "', ";
        plugin_code += "method='output" + desc.type.split("gnomon")[1] + "', ";
        plugin_code += "data_plugin='" + desc.data_plugin + "')\n";
    }
    plugin_code += "class pythonAlgorithm(gnomoncore.gnomonAbstractFormAlgorithm):\n";
    plugin_code += "    \"\"\"\n";
    plugin_code += "    Implements a custom form algorithm plugin.\n";
    plugin_code += "    \"\"\"\n";
    plugin_code += "\n";
    plugin_code += "    def __init__(self):\n";
    plugin_code += "        super().__init__()\n";
    plugin_code += "\n";
    plugin_code += "        self._parameters = {}\n";
    for (const auto &param : dd->parameters.keys()) {
        gnomonParameterDescription desc = dd->parameters[param];
        plugin_code += "        self._parameters['" + desc.name + "'] = ";
        plugin_code += dd->parameter_types[desc.type] + "(";
        plugin_code += "'" + desc.doc + "', ";
        if (desc.type == "Bool") {
            plugin_code += "True";
        } else if (desc.type == "Int") {
            plugin_code += "1, 0, 10";
        } else if (desc.type == "Double") {
            plugin_code += "1., 0., 1., 2";
        } else if (desc.type == "String") {
            plugin_code += "'', ['']";
        } else if (desc.type == "StringList") {
            plugin_code += "[''], ['']";
        }
        plugin_code += ")\n";
    }
    plugin_code += "\n";
    if (n_forms > 0) {
        for (const auto &form_type : dd->input_forms.keys()) {
            gnomonFormDescription desc = dd->input_forms[form_type];
            plugin_code += "        self." + desc.name + " = {}\n";
        }
        for (const auto &form_type : dd->output_forms.keys()) {
            gnomonFormDescription desc = dd->output_forms[form_type];
            plugin_code += "        self." + desc.name + " = {}\n";
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
        for (const auto &form_type : dd->output_forms.keys()) {
            gnomonFormDescription desc = dd->output_forms[form_type];
            plugin_code += "        self." + desc.name + " = {}\n";
        }

        if (dd->input_forms.size()>0) {
            plugin_code += "        for time in self." + dd->input_forms.values()[0].name +  ".keys():\n";
        } else {
            plugin_code += "        for time in [0]:\n";
        }
        for (const auto &form_type : dd->input_forms.keys()) {
            gnomonFormDescription desc = dd->input_forms[form_type];
            plugin_code += "            " + desc.name + " = self." + desc.name + "[time]\n";
        }
        plugin_code += "            # #}\n";
        if (run_code.isEmpty() | !run_code.startsWith("            ")) {
            plugin_code += "            # implement the run method\n";
            plugin_code += "\n";
            plugin_code += "            pass\n";
            for (const auto &form_type : dd->output_forms.keys()) {
                gnomonFormDescription desc = dd->output_forms[form_type];
                plugin_code += "            self." + desc.name + "[time] = None\n";
            }
        } else {
            plugin_code += run_code;
        }
    }

    d->editor->setText(plugin_code);
}



void gnomonPythonAlgorithmPluginEditor::parseCode(void)
{
    dd->input_forms.clear();
    dd->output_forms.clear();
    dd->parameters.clear();

    QString current_code = d->editor->toPlainText();
    dd->parser->parsePluginCode(current_code);
    
    auto input_forms = dd->parser->inputForms();
    for (const auto& form_name : input_forms.keys()) {
        this->addInputForm(input_forms[form_name], false);
    }
    auto output_forms = dd->parser->outputForms();
    for (const auto& form_name : output_forms.keys()) {
        this->addOutputForm(output_forms[form_name], false);
    }
    auto parameters = dd->parser->parameters();
    for (const auto& parameter_name : parameters.keys()) {
        this->addParameter(parameters[parameter_name], false);
    }
}



//
// gnomonPythonAlgorithmPluginEditor.cpp ends here