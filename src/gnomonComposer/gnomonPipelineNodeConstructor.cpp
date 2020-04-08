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


// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeConstructor
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeConstructor::gnomonPipelineNodeConstructor(const QString& algorithm_class, const QString& algorithm, QMap<QString, QVariant> parameters, QList<QString> outputs) : gnomonPipelineNode()
{
    d->color = QColor(83, 153, 69);

    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;
    this->parameters = parameters;

    this->setTitle(this->algorithm_class);
    this->setLabel(this->algorithm);

    for (const auto& output : outputs) {
        this->output_ports[output] = new dtkComposerScenePort(dtkComposerScenePort::Output, this);
        this->addOutputPort(this->output_ports[output]);
    }
    this->layout();
}

gnomonPipelineNodeConstructor::~gnomonPipelineNodeConstructor(void)
{

}

QString gnomonPipelineNodeConstructor::toToml(const QString& node_name)
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

QString gnomonPipelineNodeConstructor::toLuigiClass(void)
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
    out<<"        self.constructor = gnomoncore." << this->algorithm_class << "_pluginFactory().create(self.plugin_name)\n";
    out<<"        self.input_names = []\n";
    for (const auto& output_name : this->output_ports.keys()) {
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