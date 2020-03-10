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

#include "gnomonComposerSceneNode_p.h"


// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeAlgorithm
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeAlgorithm::gnomonPipelineNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QMap<QString, QVariant> parameters, QList<QString> inputs,  QList<QString> outputs) : gnomonComposerSceneNode()
{
    d->color = QColor(255, 175, 0).darker();

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

//
// gnomonPipelineNodeAlgorithm.cpp ends here