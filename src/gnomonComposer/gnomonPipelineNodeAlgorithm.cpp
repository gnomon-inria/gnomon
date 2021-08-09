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

#include "gnomonPipelineNodeAlgorithm.h"

#include "gnomonPipelineNode_p.h"


// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeAlgorithmPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelineNodeAlgorithmPrivate {
public:
    QVariantMap parameters;
    QMap<QString, dtkComposerScenePort *> input_ports;
    QMap<QString, dtkComposerScenePort *> output_ports;
};

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeAlgorithm
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeAlgorithm::gnomonPipelineNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QVariantMap parameters, QList<QString> inputs,  QList<QString> outputs) : gnomonPipelineNode(),dd(new gnomonPipelineNodeAlgorithmPrivate)
{
    if (algorithm_class.contains("From")) {
        d->color = QColor(153, 69, 69);
    } else {
        d->color = QColor(153, 125, 69);
    }
    d->algorithm_class = algorithm_class;
    d->algorithm = algorithm;

    dd->parameters = parameters;
    for (const auto& input : inputs) {
        dd->input_ports[input] = new dtkComposerScenePort(dtkComposerScenePort::Input, this);
        this->addInputPort(dd->input_ports[input]);
    }
    for (const auto& output : outputs) {
        dd->output_ports[output] = new dtkComposerScenePort(dtkComposerScenePort::Output, this);
        this->addOutputPort(dd->output_ports[output]);
    }
    this->layout();
}

gnomonPipelineNodeAlgorithm::~gnomonPipelineNodeAlgorithm(void)
{

}

const QMap<QString, dtkComposerScenePort *>& gnomonPipelineNodeAlgorithm::inputPorts(void)
{
    return dd->input_ports;
}

const QMap<QString, dtkComposerScenePort *>& gnomonPipelineNodeAlgorithm::outputPorts(void)
{
    return dd->output_ports;
}

QString gnomonPipelineNodeAlgorithm::toToml(const QString& node_name)
{
    QString node_string;
    QTextStream out(&node_string);
    out << "[" << node_name << "]" << "\n";
    out << "task_name = \""<< node_name << "\"\n";
    out << "plugin_name = \""<< d->algorithm << "\"\n";
    out << "    [" << node_name << ".parameters]\n";
    for (auto it = dd->parameters.begin(); it != dd->parameters.end(); ++it) {
        auto&& param = it.key();
        QVariant parameter = dd->parameters[param];
        QString parameter_string = d->variantParameterString(parameter);
        out << "    " << param << " = " << parameter_string << "\n";
    }
    out << "\n";
    return node_string;
}

const QJsonObject gnomonPipelineNodeAlgorithm::toJson(const QString& node_name)
{
    QJsonObject json;
    json.insert("name", node_name);
    json.insert("plugin_name", d->algorithm);
    json.insert("plugin_version", "TODO");

    QJsonObject parameters;
    for (auto it = dd->parameters.begin(); it != dd->parameters.end(); ++it) {
        auto&& param = it.key();
        QVariant parameter = dd->parameters[param];
        parameters.insert(param, QJsonValue::fromVariant(parameter));
    }
    json.insert("parameters", parameters);

    QJsonArray in;
    for (auto it = dd->input_ports.begin(); it != dd->input_ports.end(); ++it) {
        auto&& input_name = it.key();
        in.append(input_name);
    }
    json.insert("input", in);

    QJsonArray out;
    for (auto it = dd->output_ports.begin(); it != dd->output_ports.end(); ++it) {
        auto&& output_name = it.key();
        out.append(output_name);
    }
    json.insert("outputs", out);

    return json;
}

QString gnomonPipelineNodeAlgorithm::toLuigiClass(void)
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
    out<<"        self.algorithm = gnomoncore." << d->algorithm_class << "_pluginFactory().create(self.plugin_name)\n";
    out<<"        self.input_names = [";
    for (auto it = dd->input_ports.begin(); it != dd->input_ports.end(); ++it) {
        if (it != dd->input_ports.begin()) {
            out<<", ";
        }
        auto&& input_name = it.key();
        out<<"\""<<input_name<<"\"";
    }
    out<<"]\n";
    for (auto it = dd->output_ports.begin(); it != dd->output_ports.end(); ++it) {
        auto&& output_name = it.key();
        out<<"        self.form_output_functions[\"" << output_name << "\"] = self.algorithm." << output_name  <<"\n";
    }
    out<<"    \n";
    out<<"    def run(self):\n";
    out<<"        inputs = self.algorithm_inputs()\n";
    for (auto it = dd->input_ports.begin(); it != dd->input_ports.end(); ++it) {
        auto&& input_name = it.key();
        QRegularExpression numbered_input("[A-z]+[0-9]+");
        if (numbered_input.match(input_name).hasMatch()) {
            QString input_type_name = QString(input_name);
            input_type_name = input_type_name.remove(QRegularExpression("[0-9]+"));
            QString adder_name = "add" + input_type_name;
            adder_name.replace(3, 1, adder_name[3].toUpper());
            out<<"        self.algorithm." << adder_name << "(inputs[\"" << input_name << "\"])\n";
        } else {
            QString setter_name = "set" + input_name;
            setter_name.replace(3, 1, setter_name[3].toUpper());
            out<<"        self.algorithm." << setter_name << "(inputs[\"" << input_name << "\"])\n";
        }
    }
    out<<"        for name, value in self.parameters.items():\n";
    out<<"            if isinstance(value, tuple):\n";
    out<<"                value = list(value)\n";
    out<<"            self.algorithm.setParameter(name,value)\n";
    out<<"        self.algorithm.run()\n";
    out<<"\n";

    return luigi_string;
}

//
// gnomonPipelineNodeAlgorithm.cpp ends here