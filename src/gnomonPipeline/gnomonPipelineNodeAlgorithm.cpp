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

#include <dtkCore>

#include "gnomonPipelineNode_p.h"
#include "gnomonPipelinePort.h"


// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeAlgorithmPrivate
// /////////////////////////////////////////////////////////////////

class gnomonPipelineNodeAlgorithmPrivate {
public:
    QJsonObject parameters;
};

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeAlgorithm
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeAlgorithm::gnomonPipelineNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QJsonObject parameters, QList<QString> inputs,  QList<QString> outputs) : gnomonPipelineNode(),dd(new gnomonPipelineNodeAlgorithmPrivate)
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
        this->addInputPort(input, new gnomonPipelinePort(gnomonPipelinePort::Input, input, this));
    }
    for (const auto& output : outputs) {
        this->addOutputPort(output, new gnomonPipelinePort(gnomonPipelinePort::Output, output, this));
    }
    // this->layout()();
}

gnomonPipelineNodeAlgorithm::~gnomonPipelineNodeAlgorithm(void)
{

}

QJsonObject gnomonPipelineNodeAlgorithm::parameters(void)
{
    return dd->parameters;
}

QString gnomonPipelineNodeAlgorithm::toToml(void)
{
    QString node_string;
    QTextStream out(&node_string);
    out << "[" << d->name << "]" << "\n";
    out << "task_name = \""<< d->name << "\"\n";
    out << "plugin_name = \""<< d->algorithm << "\"\n";
    out << "    [" << d->name << ".parameters]\n";
    for (auto it = dd->parameters.begin(); it != dd->parameters.end(); ++it) {
        auto&& param = it.key();
        dtkCoreParameter *parameter = dtkCoreParameter::create(dd->parameters[param].toObject().toVariantHash());
        QString parameter_string = d->variantParameterString(parameter->variant());
        out << "    " << param << " = " << parameter_string << "\n";
    }
    out << "\n";
    return node_string;
}

const QJsonObject gnomonPipelineNodeAlgorithm::toJson(void)
{
    QJsonObject json = gnomonPipelineNode::toJson();

    QJsonObject parameters;
    for (auto it = dd->parameters.begin(); it != dd->parameters.end(); ++it) {
        auto&& param = it.key();
        if (param == "python_code") {
            parameters.insert(param, dd->parameters[param]);
        } else {
            QVariantHash parameter = dd->parameters[param].toObject().toVariantHash();
            parameters.insert(param, QJsonObject::fromVariantHash(parameter));
        }
    }
    json.insert("parameters", parameters);

    QJsonObject in;
    for (auto it = d->input_ports.begin(); it != d->input_ports.end(); ++it) {
        auto&& input_name = it.key();
        in.insert(input_name, QJsonValue::Null);
    }
    json.insert("inputs", in);

    QJsonArray out;
    for (auto it = d->output_ports.begin(); it != d->output_ports.end(); ++it) {
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
    for (auto it = d->input_ports.begin(); it != d->input_ports.end(); ++it) {
        if (it != d->input_ports.begin()) {
            out<<", ";
        }
        auto&& input_name = it.key();
        out<<"\""<<input_name<<"\"";
    }
    out<<"]\n";
    for (auto it = d->output_ports.begin(); it != d->output_ports.end(); ++it) {
        auto&& output_name = it.key();
        out<<"        self.form_output_functions[\"" << output_name << "\"] = self.algorithm." << output_name  <<"\n";
    }
    out<<"    \n";
    out<<"    def run(self):\n";
    out<<"        inputs = self.algorithm_inputs()\n";
    for (auto it = d->input_ports.begin(); it != d->input_ports.end(); ++it) {
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