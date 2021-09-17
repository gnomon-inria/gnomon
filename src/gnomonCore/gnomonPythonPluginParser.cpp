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


#include "gnomonPythonPluginParser.h"

#include <dtkScript>
#include <QtCore>
#include <gnomonCore>



QString stripQuotes(const QString& str)
{
    QRegExp quote_rx("[\'\"](.*)[\'\"]");
    int pos = quote_rx.indexIn(str);
    if (pos != -1) {
        return quote_rx.capturedTexts()[1];
    } else {
        return str;
    }
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
// gnomonFormDescription
// ///////////////////////////////////////////////////////////////////


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
// gnomonPythonPluginParserPrivate
// ///////////////////////////////////////////////////////////////////


class gnomonPythonPluginParserPrivate: public QObject
{
public:
    QMap<QString, gnomonFormDescription *> input_forms;
    QMap<QString, gnomonFormDescription *> output_forms;
    QMap<QString, gnomonParameterDescription *> parameters;
    
public:
    QMap<QString, QString> default_data_plugins;
    QMap<QString, QString> parameter_types;
    
public:
     gnomonPythonPluginParserPrivate(void);
    ~gnomonPythonPluginParserPrivate(void);
};

gnomonPythonPluginParserPrivate::gnomonPythonPluginParserPrivate(void)
{
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

const QMap<QString, gnomonFormDescription *>& gnomonPythonPluginParser::inputForms(void) const
{
    return d->input_forms;
}

const QMap<QString, gnomonFormDescription *>& gnomonPythonPluginParser::outputForms(void) const
{
    return d->output_forms;
}

const QMap<QString, gnomonParameterDescription *>& gnomonPythonPluginParser::parameters(void) const
{
    return d->parameters;
}

const QMap<QString, QString>& gnomonPythonPluginParser::parameterTypes(void) const
{
    return d->parameter_types;
}

void gnomonPythonPluginParser::parsePluginCode(const QString& plugin_code)
{
    d->input_forms.clear();
    d->output_forms.clear();
    d->parameters.clear();
    
    QStringList code_lines = plugin_code.split("\n");

    bool in_init = false;
    QRegExp init_rx("def[ ]*__init__[(]self");
    QRegExp method_rx("def.*[(]self");

    QRegExp input_rx("@(gnomon.*)Input[(](.*)[)]");
    QRegExp output_rx("@(gnomon.*)Output[(](.*)[)]");
    QRegExp parameter_rx("self._parameters\\[(.*)\\][ ]*=[ ]*([\\S]*)[(](.*)[)]");

    int pos = -1;
    for (const auto& line : code_lines) {
        if (init_rx.indexIn(line) != -1) {
            in_init = true;
        } else if (method_rx.indexIn(line) != -1) {
            in_init = false;
        }

        pos = input_rx.indexIn(line);
        if (pos != -1) {
            QString form_type = input_rx.capturedTexts()[1];
            QString args = input_rx.capturedTexts()[2];
            QString attr_name = stripQuotes(argumentValue(args, "attr", 0));
            QString data_plugin = stripQuotes(argumentValue(args, "data_plugin", 3));
            if (data_plugin == "") {
                data_plugin = d->default_data_plugins[form_type];
            }
            d->input_forms[attr_name] = new gnomonFormDescription(attr_name, form_type, data_plugin);
        }

        pos = output_rx.indexIn(line);
        if (pos != -1) {
            QString form_type = output_rx.capturedTexts()[1];
            QString args = output_rx.capturedTexts()[2];
            QString attr_name = stripQuotes(argumentValue(args, "attr", 0));
            QString data_plugin = stripQuotes(argumentValue(args, "data_plugin", 2));
            if (data_plugin == "") {
                data_plugin = d->default_data_plugins[form_type];
            }
            d->output_forms[attr_name] = new gnomonFormDescription(attr_name, form_type, data_plugin);
        }

        if (in_init) {
            pos = parameter_rx.indexIn(line);
            if (pos != -1) {
                QString parameter_name = stripQuotes(parameter_rx.capturedTexts()[1]);
                QString parameter_type = d->parameter_types.key(parameter_rx.capturedTexts()[2]);
                QString parameter_args = parameter_rx.capturedTexts()[3];
                QString parameter_doc = stripQuotes(argumentValue(parameter_args, "documentation", 0));
                d->parameters[parameter_name] = new gnomonParameterDescription(parameter_name, parameter_type, parameter_doc);
            }
        }
    }
}