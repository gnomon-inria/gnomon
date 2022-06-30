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

#include "gnomonPipelineNodeAdapter.h"

#include "gnomonPipelineNode_p.h"
#include "gnomonPipelinePort.h"

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeAdapter
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeAdapter::gnomonPipelineNodeAdapter(const QString& algorithm_class, const QString& algorithm, QList<QString> inputs,  QList<QString> outputs) : gnomonPipelineNode()
{
    d->type = gnomonPipelineNode::NODE_ADAPTER;
    d->algorithm_class = algorithm_class;
    d->algorithm = algorithm;

    for (const auto& input : inputs) {
        this->addInputPort(input, new gnomonPipelinePort(gnomonPipelinePort::Input, input, this));
    }
    for (const auto& output : outputs) {
        this->addOutputPort(output, new gnomonPipelinePort(gnomonPipelinePort::Output, output, this));
    }
    // this->layout()();
}

gnomonPipelineNodeAdapter::~gnomonPipelineNodeAdapter(void)
{

}

QString gnomonPipelineNodeAdapter::toToml(void)
{
    QString node_string;
    QTextStream out(&node_string);
    out << "[" << d->name << "]" << "\n";
    out << "task_name = \""<< d->name << "\"\n";
    out << "plugin_name = \""<< d->algorithm << "\"\n";
    out << "\n";
    return node_string;
}

const QJsonObject gnomonPipelineNodeAdapter::toJson(void)
{
    QJsonObject json = gnomonPipelineNode::toJson();
    return json;
}
QString gnomonPipelineNodeAdapter::toLuigiClass(void)
{
    QString luigi_string;
    QTextStream out(&luigi_string);

    QString class_name = QString(d->algorithm_class) + "Task";
    class_name.replace(0, 1, class_name[0].toUpper());

    out<<"\n";
    out<<"class "<<class_name<<"(AlgorithmPluginTask):\n";
    out<<"    \n";
    out<<"    def __init__(self, **kwargs):\n";
    out<<"        super().__init__(**kwargs)\n";
    out<<"        load_plugin_group(\"" << d->algorithm_class << "\")\n";
    out<<"        self.adapter = gnomoncore." << d->algorithm_class << "_pluginFactory().create(self.plugin_name)\n";
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
        out<<"        self.form_output_functions[\"" << output_name << "\"] = self.adapter." << output_name  <<"\n";
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
            out<<"        self.adapter." << adder_name << "(inputs[\"" << input_name << "\"])\n";
        } else {
            QString setter_name = "set" + input_name;
            setter_name.replace(3, 1, setter_name[3].toUpper());
            out<<"        self.adapter." << setter_name << "(inputs[\"" << input_name << "\"])\n";
        }
    }
    out<<"        self.adapter.run()\n";
    out<<"\n";

    return luigi_string;
}

//
// gnomonPipelineNodeAdapter.cpp ends here