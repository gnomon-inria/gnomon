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
// gnomonPipelineNodeAlgorithm
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeAlgorithm::gnomonPipelineNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QMap<QString, QVariant> parameters, QList<QString> inputs,  QList<QString> outputs) : gnomonPipelineNode()
{
    if (algorithm_class.contains("From")) {
        d->color = QColor(255, 0, 0);
    } else {
        d->color = QColor(255, 170, 0);
    }

    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;
    this->parameters = parameters;

    this->setTitle(this->algorithm_class);

    for (const auto& input : inputs) {
        this->input_ports[input] = new dtkComposerScenePort(dtkComposerScenePort::Input, this);
        this->addInputPort(this->input_ports[input]);
    }
    for (const auto& output : outputs) {
        this->output_ports[output] = new dtkComposerScenePort(dtkComposerScenePort::Output, this);
        this->addOutputPort(this->output_ports[output]);
    }
    this->layout();
}

gnomonPipelineNodeAlgorithm::~gnomonPipelineNodeAlgorithm(void)
{

}

QString gnomonPipelineNodeAlgorithm::toToml(const QString& node_name)
{
    QString node_string;
    QTextStream out(&node_string);
    out << "[" << node_name << "]" << "\n";
    out << "task_name = \""<< node_name << "\"\n";
    out << "plugin_name = \""<< this->algorithm << "\"\n";
    out << "    [" << node_name << ".parameters]\n";
    for (const auto& param : this->parameters.keys()) {
        QVariant parameter = this->parameters[param];
        QString parameter_string = this->variantParameterString(parameter);
        out << "    " << param << " = " << parameter_string << "\n";
    }
    out << "\n";
    return node_string;
}

QString gnomonPipelineNodeAlgorithm::toLuigiClass(void)
{
    QString luigi_string;
    QTextStream out(&luigi_string);

    QString class_name = QString(this->algorithm_class) + "Task";
    class_name.replace(0, 1, class_name[0].toUpper());

    out<<"\n";
    out<<"class "<<class_name<<"(AlgorithmPluginTask):\n";
    out<<"    parameters = luigi.DictParameter()\n";
    out<<"    \n";
    out<<"    def __init__(self, **kwargs):\n";
    out<<"        super().__init__(**kwargs)\n";
    out<<"        load_plugin_group(\"" << this->algorithm_class << "\")\n";
    out<<"        self.algorithm = gnomoncore." << this->algorithm_class << "_pluginFactory().create(self.plugin_name)\n";
    out<<"        self.input_names = [";
    for (int i=0; i<this->input_ports.size(); i++) {
        if (i>0) {
            out<<", ";
        }
        QString input_name = this->input_ports.keys()[i];
        out<<"\""<<input_name<<"\"";
    }
    out<<"]\n";
    for (const auto& output_name : this->output_ports.keys()) {
        out<<"        self.form_output_functions[\"" << output_name << "\"] = self.algorithm." << output_name  <<"\n";
    }
    out<<"    \n";
    out<<"    def run(self):\n";
    out<<"        inputs = self.algorithm_inputs()\n";
    for (const auto& input_name : this->input_ports.keys()) {
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
    out<<"            self.algorithm.setParameter(name,value)\n";
    out<<"        self.algorithm.run()\n";
    out<<"\n";

    return luigi_string;
}

//
// gnomonPipelineNodeAlgorithm.cpp ends here