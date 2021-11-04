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

#include "gnomonPipelineNodeConstructor.h"

#include "gnomonPipelineNode_p.h"
#include "gnomonPipelinePort.h"


// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeConstructorPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelineNodeConstructorPrivate {
public:
    QVariantMap parameters;
};

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeConstructor
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeConstructor::gnomonPipelineNodeConstructor(const QString& algorithm_class, const QString& algorithm, QVariantMap parameters, QList<QString> outputs) : gnomonPipelineNode(), dd(new gnomonPipelineNodeConstructorPrivate)
{
    d->color = QColor(83, 153, 69);

    d->algorithm_class = algorithm_class;
    d->algorithm = algorithm;

    dd->parameters = parameters;
    for (const auto& output : outputs) {
        this->addOutputPort(output, new gnomonPipelinePort(gnomonPipelinePort::Output, output, this));
    }
    // this->layout()();
}

gnomonPipelineNodeConstructor::~gnomonPipelineNodeConstructor(void)
{

}

QString gnomonPipelineNodeConstructor::toToml(void)
{
    QString node_string;
    QTextStream out(&node_string);
    out << "[" << d->name << "]" << "\n";
    out << "task_name = \""<< d->name << "\"\n";
    out << "plugin_name = \""<< d->algorithm << "\"\n";
    out << "    [" << d->name << ".parameters]\n";
    for (auto it = dd->parameters.begin(); it != dd->parameters.end(); ++it) {
        auto&& param = it.key();
        QVariant parameter = dd->parameters[param];
        QString parameter_string = d->variantParameterString(parameter);
        out << "    " << param << " = " << parameter_string << "\n";
    }
    out << "\n";
    return node_string;
}

const QJsonObject gnomonPipelineNodeConstructor::toJson(void)
{
    QJsonObject json;
    json.insert("name", d->name);
    json.insert("plugin_name", d->algorithm);

    QJsonObject parameters;
    for (auto it = dd->parameters.begin(); it != dd->parameters.end(); ++it) {
        auto&& param = it.key();
        QVariant parameter = dd->parameters[param];
        parameters.insert(param, QJsonValue::fromVariant(parameter));
    }
    json.insert("parameters", parameters);

    QJsonArray out;
    for (auto it = d->output_ports.begin(); it != d->output_ports.end(); ++it) {
        auto&& output_name = it.key();
        out.append(output_name);
    }
    json.insert("outputs", out);

    return json;
}

QString gnomonPipelineNodeConstructor::toLuigiClass(void)
{
    QString luigi_string;
    QTextStream out(&luigi_string);

    QString class_name = QString(d->algorithm_class) + "Task";
    class_name.replace(0, 1, class_name[0].toUpper());

    out<<"\n";
    out<<"class "<<class_name<<"(AlgorithmPluginTask):\n";
    out<<"    parameters = luigi.DictParameter()\n";
    out<<"    \n";
    out<<"    def __init__(self, **kwargs):\n";
    out<<"        super().__init__(**kwargs)\n";
    out<<"        load_plugin_group(\"" << d->algorithm_class << "\")\n";
    out<<"        self.constructor = gnomoncore." << d->algorithm_class << "_pluginFactory().create(self.plugin_name)\n";
    out<<"        self.input_names = []\n";
    for (auto it = d->output_ports.begin(); it != d->output_ports.end(); ++it) {
        auto&& output_name = it.key();
        out<<"        self.form_output_functions[\"" << output_name << "\"] = self.constructor." << output_name  <<"\n";
    }
    out<<"    \n";
    out<<"    def run(self):\n";
    out<<"        for name, value in self.parameters.items():\n";
    out<<"            if isinstance(value, tuple):\n";
    out<<"                value = list(value)\n";
    out<<"            self.constructor.setParameter(name,value)\n";
    out<<"        self.constructor.run()\n";
    out<<"\n";

    return luigi_string;
}

//
// gnomonPipelineNodeConstructor.cpp ends here