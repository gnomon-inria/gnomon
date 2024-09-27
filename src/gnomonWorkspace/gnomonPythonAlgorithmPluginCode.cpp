#include "gnomonPythonAlgorithmPluginCode.h"

#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonAbstractBinaryImageData>
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonAbstractCellComplexData>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonAbstractCellImageData>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonAbstractImageData>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonAbstractMeshData>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonAbstractPointCloudData>

class gnomonPythonAlgorithmPluginCodePrivate
{
public:
    QString text;

public:
    QString plugin_class_name;
    QString plugin_name;
    QString plugin_documentation;
    QString file_name;

public:
    QMap<QString, QString> parameter_types;
    QMap<QString, QStringList> form_data_plugins;

public:
    QMap<QString, gnomonFormDescription> input_forms;
    QMap<QString, gnomonFormDescription> output_forms;
    QMap<QString, gnomonParameterDescription> parameters;


public:
    gnomonPythonPluginParser *parser;

public:
     gnomonPythonAlgorithmPluginCodePrivate(void);
    ~gnomonPythonAlgorithmPluginCodePrivate(void);
};

gnomonPythonAlgorithmPluginCodePrivate::gnomonPythonAlgorithmPluginCodePrivate(void)
{
    this->plugin_class_name = "pythonAlgorithm";
    this->plugin_name = "Custom Algorithm";
    this->plugin_documentation = "Implements a custom form algorithm plugin.";

    this->parser = new gnomonPythonPluginParser();

    this->parameter_types = this->parser->parameterTypes();
}


gnomonPythonAlgorithmPluginCodePrivate::~gnomonPythonAlgorithmPluginCodePrivate(void)
{
}

// ///////////////////////////////////////////////////////////////////
// gnomonPythonAlgorithmPluginCode
// ///////////////////////////////////////////////////////////////////

gnomonPythonAlgorithmPluginCode::gnomonPythonAlgorithmPluginCode(QObject *parent) : QObject(parent)
{
    d = new gnomonPythonAlgorithmPluginCodePrivate();
}

gnomonPythonAlgorithmPluginCode::~gnomonPythonAlgorithmPluginCode(void)
{
    delete d;
}

QString gnomonPythonAlgorithmPluginCode::text(void)
{
    return d->text;
}

void gnomonPythonAlgorithmPluginCode::setText(const QString& text)
{
    if (text != d->text) {
        d->text = text;
        emit textChanged(d->text);
    }
}

QString gnomonPythonAlgorithmPluginCode::fileName(void)
{
    return d->file_name;
}

void gnomonPythonAlgorithmPluginCode::setFileName(const QString& name)
{
    if(name != d->file_name) {
        d->file_name = name;
        emit fileChanged(d->file_name);
    }
}

const QString& gnomonPythonAlgorithmPluginCode::pluginClassName(void) const
{
    return d->plugin_class_name;
}

void gnomonPythonAlgorithmPluginCode::setPluginClassName(const QString& name, bool update_code)
{
    if (name != d->plugin_class_name) {
        d->plugin_class_name = name;
        if (update_code) {
            this->updateCode();
        }
        emit pluginClassNameChanged();
    }
}

const QString& gnomonPythonAlgorithmPluginCode::pluginName(void) const
{
    return d->plugin_name;
}

void gnomonPythonAlgorithmPluginCode::setPluginName(const QString& name, bool update_code)
{
    if (name != d->plugin_name) {
        d->plugin_name = name;
        if (update_code) {
            this->updateCode();
        }
        emit pluginNameChanged();
    }
}

const QString& gnomonPythonAlgorithmPluginCode::pluginDocumentation(void) const
{
    return d->plugin_documentation;
}

void gnomonPythonAlgorithmPluginCode::setPluginDocumentation(const QString& doc, bool update_code)
{
    if (doc != d->plugin_documentation) {
        d->plugin_documentation = doc;
        if (update_code) {
            this->updateCode();
        }
        emit pluginDocumentationChanged();
    }
}

QJsonObject gnomonPythonAlgorithmPluginCode::inputFormsJson(void) const
{
    QJsonObject forms;
    for (const auto& form_name : d->input_forms.keys()) {
        QJsonObject formDescription;
        formDescription.insert("name", d->input_forms[form_name].name);
        formDescription.insert("type", d->input_forms[form_name].type);
        formDescription.insert("data_plugin", d->input_forms[form_name].data_plugin);
        forms.insert(form_name, formDescription);
    }
    return forms;
}

QJsonObject gnomonPythonAlgorithmPluginCode::outputFormsJson(void) const
{
    QJsonObject forms;
    for (const auto& form_name : d->output_forms.keys()) {
        QJsonObject formDescription;
        formDescription.insert("name", d->output_forms[form_name].name);
        formDescription.insert("type", d->output_forms[form_name].type);
        formDescription.insert("data_plugin", d->output_forms[form_name].data_plugin);
        forms.insert(form_name, formDescription);
    }
    return forms;
}

QJsonObject gnomonPythonAlgorithmPluginCode::parametersJson(void) const
{
    QJsonObject parameters;
    for (const auto& parameter_name : d->parameters.keys()) {
        QJsonObject parameterDescription;
        parameterDescription.insert("name", d->parameters[parameter_name].name);
        parameterDescription.insert("type", d->parameters[parameter_name].type);
        parameterDescription.insert("doc", d->parameters[parameter_name].doc);
        parameterDescription.insert("value", d->parameters[parameter_name].value);
        parameterDescription.insert("args", d->parameters[parameter_name].args);
        parameters.insert(parameter_name, parameterDescription);
    }
    return parameters;
}

const QMap<QString, gnomonFormDescription>& gnomonPythonAlgorithmPluginCode::inputForms(void) const
{
    return d->input_forms;
}

const QMap<QString, gnomonFormDescription>& gnomonPythonAlgorithmPluginCode::outputForms(void) const
{
    return d->output_forms;
}

const QMap<QString, gnomonParameterDescription>& gnomonPythonAlgorithmPluginCode::parameters(void) const
{
    return d->parameters;
}

void gnomonPythonAlgorithmPluginCode::updateDataPlugins(const QString& form_type)
{
    d->form_data_plugins[form_type] = this->formDataPlugins(form_type);
}

QStringList gnomonPythonAlgorithmPluginCode::formDataPlugins(const QString& form_type)
{
    QStringList factory_keys;
    if (form_type == "gnomonBinaryImage") {
        factory_keys = gnomonCore::binaryImageData::pluginFactory().keys();
    } else if (form_type == "gnomonCellComplex") {
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
    return factory_keys;
}

QStringList gnomonPythonAlgorithmPluginCode::formTypes() const
{
    return d->parser->defaultFormDataPlugins().keys();
}

QStringList gnomonPythonAlgorithmPluginCode::parameterTypes() const
{
    return d->parser->parameterTypes().keys();
}

void gnomonPythonAlgorithmPluginCode::addInputForm(gnomonFormDescription desc, bool update_code)
{
    d->input_forms[desc.type] = desc;
    if (update_code) {
        this->updateCode();
    }
    emit inputFormsChanged();
}

void gnomonPythonAlgorithmPluginCode::addInputForm(const QJsonObject& desc)
{
    QString name = desc.contains("name") ? desc["name"].toString() : "";
    QString type = desc.contains("type") ? desc["type"].toString() : "";
    QString data_plugin = desc.contains("data_plugin") ? desc["data_plugin"].toString() : "";
    this->addInputForm(gnomonFormDescription(name, type, data_plugin), true);
}

void gnomonPythonAlgorithmPluginCode::removeInputForm(const QJsonObject& desc)
{
    d->input_forms.remove(desc["type"].toString());
    this->updateCode();
    emit inputFormsChanged();
}

void gnomonPythonAlgorithmPluginCode::addOutputForm(gnomonFormDescription desc, bool update_code)
{
    d->output_forms[desc.type] = desc;
    if (update_code) {
        this->updateCode();
    }
    emit outputFormsChanged();
}

void gnomonPythonAlgorithmPluginCode::addOutputForm(const QJsonObject& desc)
{
    QString name = desc.contains("name") ? desc["name"].toString() : "";
    QString type = desc.contains("type") ? desc["type"].toString() : "";
    QString data_plugin = desc.contains("data_plugin") ? desc["data_plugin"].toString() : "";
    this->addOutputForm(gnomonFormDescription(name, type, data_plugin), true);
}

void gnomonPythonAlgorithmPluginCode::removeOutputForm(const QJsonObject& desc)
{
    d->output_forms.remove(desc["type"].toString());
    this->updateCode();
    emit outputFormsChanged();
}

void gnomonPythonAlgorithmPluginCode::addParameter(gnomonParameterDescription desc, bool update_code)
{
    d->parameters[desc.name] = desc;
    if (update_code) {
        this->updateCode();
    }
    emit parametersChanged();
}

void gnomonPythonAlgorithmPluginCode::addParameter(const QJsonObject& desc)
{
    QString name = desc.contains("name") ? desc["name"].toString() : "";
    QString type = desc.contains("type") ? desc["type"].toString() : "";
    QString doc = desc.contains("doc") ? desc["doc"].toString() : "";
    QString value = desc.contains("value") ? desc["value"].toString() : "";
    QJsonObject args  = desc.contains("args") ? desc["args"].toObject() : QJsonObject();

    this->addParameter(gnomonParameterDescription(name, type, doc, value, args), true);
}

void gnomonPythonAlgorithmPluginCode::removeParameter(const QJsonObject& desc)
{
    d->parameters.remove(desc["name"].toString());
    this->updateCode();
    emit parametersChanged();
}

void gnomonPythonAlgorithmPluginCode::updateCode(void)
{
    QString current_code = this->text();

    QStringList code_lines = current_code.split("\n");
    QString import_code = "";
    QString run_code = "";

    bool user_line = false;
    int section = 0;

    if (current_code.contains("# {# gnomon")) { // Well-formatted plugin
        for (const auto &line: code_lines) {
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
    } else { // Have to guess!
        for (const auto &line: code_lines) {
            if (line.contains("import")) {
                user_line = true;
                section = 1;
            } else if (line.contains("@")) {
                user_line = false;
            }
            if (user_line) {
                if (section == 1) {
                    import_code += line + "\n";
                } else {
                    run_code += line + "\n";
                }
            }
            if (line.contains("def run(")) {
                user_line = true;
                section = 2;
            }
        }
    }
    run_code = run_code.left(run_code.length()-1);

    QString plugin_code = "";

    plugin_code += "# {# gnomon, plugin.imports\n";
    plugin_code += "# do not modify, any code after the gnomon tag will be overwritten\n";

    plugin_code += "from dtkcore import d_bool, d_int, d_real, d_inliststring, d_inliststringlist\n";
    plugin_code += "\n";
    plugin_code += "import gnomon.core\n";
    plugin_code += "\n";
    plugin_code += "from gnomon.utils import algorithmPlugin\n";

    if (d->input_forms.size() + d->output_forms.size() > 0) {
        plugin_code += "from gnomon.utils.decorators import";
    }
    int n_forms = 0;
    for (const auto &form_type : d->input_forms.keys()) {
        if (n_forms > 0) {
            plugin_code += ",";
        }
        plugin_code += " " + unCapitalize(form_type.split("gnomon")[1]) + "Input";
        n_forms ++;
    }
    for (const auto &form_type : d->output_forms.keys()) {
        if (n_forms > 0) {
            plugin_code += ",";
        }
        plugin_code += " " + unCapitalize(form_type.split("gnomon")[1]) + "Output";
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
    plugin_code += "# do not modify, any code after the gnomon tag will be overwritten\n";

    plugin_code += "@algorithmPlugin(version='0.1.0', coreversion='1.0.2', name='" + d->plugin_name + "')\n";

    for (const auto &form_type : d->input_forms.keys()) {
        gnomonFormDescription desc = d->input_forms[form_type];
        plugin_code += "@" + unCapitalize(form_type.split("gnomon")[1]) + "Input(";
        plugin_code += "attr='" + desc.name + "', ";
        plugin_code += "data_plugin='" + desc.data_plugin + "')\n";
    }
    for (const auto &form_type : d->output_forms.keys()) {
        gnomonFormDescription desc = d->output_forms[form_type];
        plugin_code += "@" + unCapitalize(form_type.split("gnomon")[1]) + "Output(";
        plugin_code += "attr='" + desc.name + "', ";
        plugin_code += "data_plugin='" + desc.data_plugin + "')\n";
    }
    plugin_code += "class " + d->plugin_class_name + "(gnomon.core.gnomonAbstractFormAlgorithm):\n";
    plugin_code += "    \"\"\"";
    QStringList doc_lines = d->plugin_documentation.split("\n");
    for (int i=0; i<doc_lines.size(); i++) {
        QString doc_line = doc_lines[i];
        plugin_code += (i>0 ? "    " : "") + doc_line + "\n";
    }
    plugin_code += "\n";
    plugin_code += "    \"\"\"\n";
    plugin_code += "\n";
    plugin_code += "    def __init__(self):\n";
    plugin_code += "        super().__init__()\n";
    plugin_code += "\n";
    plugin_code += "        self._parameters = {}\n";
    for (const auto &param : d->parameters.keys()) {
        gnomonParameterDescription desc = d->parameters[param];
        plugin_code += "        self._parameters['" + desc.name + "'] = ";
        plugin_code += d->parameter_types[desc.type] + "(";
        plugin_code += "'" + desc.args["label"].toString() + "', ";
        if (desc.type == "Bool") {
            plugin_code += desc.value;
        } else if (desc.type == "Int") {
            if(!desc.args.isEmpty()) {
                plugin_code += desc.value+", "+desc.args["min"].toString()+", "+desc.args["max"].toString();
            } else {
                plugin_code += "1, 0, 10";
            }
        } else if (desc.type == "Double") {
            if (!desc.args.isEmpty()) {
                plugin_code += desc.value+", "+desc.args["min"].toString()+", "+desc.args["max"].toString()+", "+desc.args["decimals"].toString();
            } else {
                plugin_code += "1., 0., 10., 1.";
            }
        } else if (desc.type == "String") {
            if (!desc.args.isEmpty()) {
                plugin_code += desc.value+", "+desc.args["list"].toString();
            } else {
                plugin_code += "'', ['']";
            }
        } else if (desc.type == "StringList") {
            if (!desc.args.isEmpty()) {
                plugin_code += desc.value+", "+desc.args["list"].toString();
            } else {
                plugin_code += "[''], ['']";
            }
        }
        plugin_code += ", '" + desc.doc + "' ";
        plugin_code += ")\n";
    }
    plugin_code += "\n";
    if (n_forms > 0) {
        for (const auto &form_type : d->input_forms.keys()) {
            gnomonFormDescription desc = d->input_forms[form_type];
            plugin_code += "        self." + desc.name + " = {}\n";
        }
        for (const auto &form_type : d->output_forms.keys()) {
            gnomonFormDescription desc = d->output_forms[form_type];
            plugin_code += "        self." + desc.name + " = {}\n";
        }
        plugin_code += "\n";
    }
    plugin_code += "    def run(self):\n";

    QString default_run_code =  "        # implement the run method\n\n        pass\n";
    QString default_run_code_indented =  "            # implement the run method\n\n            pass\n";

    if ((n_forms == 0) || (run_code.startsWith("        ") && !run_code.contains(default_run_code))) {
        plugin_code += "        # #}\n";
        if (run_code.isEmpty() || run_code.startsWith(default_run_code_indented) || run_code.startsWith(default_run_code)) {
            plugin_code += default_run_code;
        } else {
            plugin_code += run_code;
        }
    } else {
        for (const auto &form_type : d->output_forms.keys()) {
            gnomonFormDescription desc = d->output_forms[form_type];
            plugin_code += "        self." + desc.name + " = {}\n";
        }

        if (d->input_forms.size()>0) {
            plugin_code += "        self.set_max_progress(len(self." + d->input_forms.values()[0].name +")) # Should be equal to the number of progress increments\n";
            plugin_code += "        for time in self." + d->input_forms.values()[0].name +  ".keys():\n";
        } else {
            plugin_code += "        self.set_max_progress(1) # Should be equal to the number of progress increments\n";
            plugin_code += "        for time in [0]:\n";
        }
        plugin_code += "            self.set_progress_message(f'T {time} : Running')\n";
        for (const auto &form_type : d->input_forms.keys()) {
            gnomonFormDescription desc = d->input_forms[form_type];
            plugin_code += "            " + desc.name + " = self." + desc.name + "[time]\n";
        }
        plugin_code += "            self.increment_progress()\n";
        plugin_code += "            # #}\n";
        if (run_code.isEmpty() || run_code.startsWith(default_run_code_indented) || run_code.startsWith(default_run_code)) {
            plugin_code += default_run_code_indented;
            for (const auto &form_type : d->output_forms.keys()) {
                gnomonFormDescription desc = d->output_forms[form_type];
                plugin_code += "            self." + desc.name + "[time] = None\n";
            }
        } else {
            plugin_code += run_code;
        }
    }

    if(d->file_name.isEmpty())
        d->file_name = "example.py";

    this->setText(plugin_code);
    emit codeUpdated();
}

void gnomonPythonAlgorithmPluginCode::parseCode(void)
{
    d->input_forms.clear();
    emit inputFormsChanged();
    d->output_forms.clear();
    emit outputFormsChanged();
    d->parameters.clear();
    emit parametersChanged();

    QString current_code = this->text();
    d->parser->parsePluginCode(current_code);

    this->setPluginName(d->parser->pluginName(), false);
    this->setPluginDocumentation(d->parser->pluginDocumentation(), false);

    auto input_forms = d->parser->inputForms();
    for (const auto& form_name : input_forms.keys()) {
        this->addInputForm(input_forms[form_name], false);
    }
    auto output_forms = d->parser->outputForms();
    for (const auto& form_name : output_forms.keys()) {
        this->addOutputForm(output_forms[form_name], false);
    }
    auto parameters = d->parser->parameters();
    for (const auto& parameter_name : parameters.keys()) {
        this->addParameter(parameters[parameter_name], false);
    }
}


//
// gnomonPythonAlgorithmPluginCode.cpp ends here
