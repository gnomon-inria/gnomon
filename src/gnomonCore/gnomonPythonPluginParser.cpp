
#include "gnomonPythonPluginParser.h"

#include <dtkScript>
#include <QtCore>

QString stripQuotes(const QString& str)
{
    QRegularExpression quote_rx("[\'\"](.*)[\'\"]");
    auto match = quote_rx.match(str);
    if (match.hasMatch()) {
        return match.capturedTexts()[1];
    } else {
        return str;
    }
}

QString capitalize(const QString& str)
{
    QString cap = str.at(0).toUpper() + str.mid(1);
    return cap;
}

QString unCapitalize(const QString& str)
{
    QString cap = str.at(0).toLower() + str.mid(1);
    return cap;
}

QString argumentValue(const QString& arguments, const QString& argument_name, int argument_position)
{
    QStringList args = arguments.split(",");
    QString arg_value("");
    int arg_pos = 0;
    for (const auto& arg : args) {
        if (arg.contains("=")) {
            QString arg_name = arg.split("=")[0];
            if (arg_name == argument_name) {
                arg_value = arg.split("=")[1];
            }
        } else if (arg_pos == argument_position) {
            arg_value = arg;
        } else {
            arg_pos += 1;
        }
    }
    return arg_value;
}


// ///////////////////////////////////////////////////////////////////
// gnomonPythonPluginParserPrivate
// ///////////////////////////////////////////////////////////////////


class gnomonPythonPluginParserPrivate: public QObject
{
public:
    QString plugin_name;
    QString plugin_documentation;

public:
    QMap<QString, gnomonFormDescription> input_forms;
    QMap<QString, gnomonFormDescription> output_forms;
    QMap<QString, gnomonParameterDescription> parameters;
    
public:
    QMap<QString, QString> default_data_plugins;
    QMap<QString, QString> parameter_types;
    
public:
     gnomonPythonPluginParserPrivate(void);
    ~gnomonPythonPluginParserPrivate(void);
};

gnomonPythonPluginParserPrivate::gnomonPythonPluginParserPrivate(void)
{
    this->default_data_plugins["gnomonBinaryImage"] = "gnomonBinaryImageDataSpatialImage";
    this->default_data_plugins["gnomonCellComplex"] = "gnomonCellComplexDataPropertyTopomesh";
    this->default_data_plugins["gnomonCellImage"] = "gnomonCellImageDataPropertySpatialImage";
    this->default_data_plugins["gnomonImage"] = "gnomonImageDataSpatialImageDict";
    this->default_data_plugins["gnomonMesh"] = "gnomonMeshDataPropertyTopomesh";
    this->default_data_plugins["gnomonPointCloud"] = "gnomonPointCloudDataPropertyTopomesh";

    this->parameter_types["Bool"] = "d_bool";
    this->parameter_types["Int"] = "d_int";
    this->parameter_types["Double"] = "d_real";
    this->parameter_types["String"] = "d_inliststring";
    this->parameter_types["StringList"] = "d_inliststringlist";
}

gnomonPythonPluginParserPrivate::~gnomonPythonPluginParserPrivate(void)
{
}

// ///////////////////////////////////////////////////////////////////
// gnomonPythonPluginParser
// ///////////////////////////////////////////////////////////////////

gnomonPythonPluginParser::gnomonPythonPluginParser(void)
{
    d = new gnomonPythonPluginParserPrivate;
}

gnomonPythonPluginParser::~gnomonPythonPluginParser(void)
{
    delete d;
}

const QString& gnomonPythonPluginParser::pluginName(void) const
{
    return d->plugin_name;
}

const QString& gnomonPythonPluginParser::pluginDocumentation(void) const
{
    return d->plugin_documentation;
}

const QMap<QString, gnomonFormDescription>& gnomonPythonPluginParser::inputForms(void) const
{
    return d->input_forms;
}

const QMap<QString, gnomonFormDescription>& gnomonPythonPluginParser::outputForms(void) const
{
    return d->output_forms;
}

const QMap<QString, gnomonParameterDescription>& gnomonPythonPluginParser::parameters(void) const
{
    return d->parameters;
}

const QMap<QString, QString>& gnomonPythonPluginParser::parameterTypes(void) const
{
    return d->parameter_types;
}

const QMap<QString, QString>& gnomonPythonPluginParser::defaultFormDataPlugins(void) const
{
    return d->default_data_plugins;
}

void gnomonPythonPluginParser::parsePluginCode(const QString& plugin_code)
{
    d->input_forms.clear();
    d->output_forms.clear();
    d->parameters.clear();
    
    QStringList code_lines = plugin_code.split("\n");

    bool in_init = false;
    bool in_docstring = false;
    bool docstring_read = false;

    d->plugin_documentation = "";

    QRegularExpression plugin_rx("class (.*)[(]gnomon");
    QRegularExpression docstring_rx("(\"\"\"|''')(.*)");

    QRegularExpression init_rx("def[ ]*__init__[(]self");
    QRegularExpression method_rx("def.*[(]self");

    QRegularExpression input_rx("@(.*)Input[(](.*)[)]");
    QRegularExpression output_rx("@(.*)Output[(](.*)[)]");
    QRegularExpression parameter_rx("self._parameters\\[(.*)\\][ ]*=[ ]*([\\S]*)[(](.*)[)]");

    for (const auto& line : code_lines) {
        if (init_rx.match(line).hasMatch()) {
            in_init = true;
        } else if (method_rx.match(line).hasMatch()) {
            in_init = false;
        }

        if (in_docstring) {
            if (docstring_rx.match(line).hasMatch()) {
                if (line.startsWith("    ")) {
                    d->plugin_documentation += line.mid(4) + "\n";
                } else {
                    d->plugin_documentation += line + "\n";
                }
            } else {
                in_docstring = false;
                docstring_read = true;
            }
        } else {
            auto match = docstring_rx.match(line);
            if ((match.hasMatch()) & (!docstring_read)) {
                in_docstring = true;
                d->plugin_documentation += match.capturedTexts()[2] + "\n";
            }
        }

        auto match = plugin_rx.match(line);
        if (match.hasMatch()) {
            d->plugin_name = match.capturedTexts()[1];
        }

        match = input_rx.match(line);
        if (match.hasMatch()) {
            QString form_type = "gnomon" + capitalize(match.capturedTexts()[1]);
            QString args = match.capturedTexts()[2];
            QString attr_name = stripQuotes(argumentValue(args, "attr", 0));
            QString data_plugin = stripQuotes(argumentValue(args, "data_plugin", 3));
            if (data_plugin == "") {
                data_plugin = d->default_data_plugins[form_type];
            }
            d->input_forms[attr_name] = gnomonFormDescription(attr_name, form_type, data_plugin);
        }

        match = output_rx.match(line);
        if (match.hasMatch()) {
            QString form_type = "gnomon" + capitalize(match.capturedTexts()[1]);
            QString args = match.capturedTexts()[2];
            QString attr_name = stripQuotes(argumentValue(args, "attr", 0));
            QString data_plugin = stripQuotes(argumentValue(args, "data_plugin", 2));
            if (data_plugin == "") {
                data_plugin = d->default_data_plugins[form_type];
            }
            d->output_forms[attr_name] = gnomonFormDescription(attr_name, form_type, data_plugin);
        }

        if (in_init) {
            match = parameter_rx.match(line);
            if (match.hasMatch()) {
                QString parameter_name = stripQuotes(match.capturedTexts()[1]);
                QString parameter_type = d->parameter_types.key(match.capturedTexts()[2]);
                QString parameter_args = match.captured()[3];
                QString parameter_doc = stripQuotes(argumentValue(parameter_args, "documentation", 0));
                d->parameters[parameter_name] = gnomonParameterDescription(parameter_name, parameter_type, parameter_doc);
            }
        }
    }
    if (d->plugin_documentation.startsWith('\n')) {
        d->plugin_documentation.remove(0, 1);
    }
    if (d->plugin_documentation.endsWith('\n')) {
        d->plugin_documentation.remove(d->plugin_documentation.size()-1, 1);
    }
    if (d->plugin_documentation.endsWith('\n')) {
        d->plugin_documentation.remove(d->plugin_documentation.size()-1, 1);
    }
}
