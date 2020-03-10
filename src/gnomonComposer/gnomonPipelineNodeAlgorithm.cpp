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

//
// gnomonPipelineNodeAlgorithm.cpp ends here