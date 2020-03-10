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

#include "gnomonPipelineNodeWriter.h"

#include "gnomonComposerSceneNode_p.h"

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeWriter
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeWriter::gnomonPipelineNodeWriter(const QString& algorithm_class, const QString& algorithm, const QString& path) : gnomonComposerSceneNode()
{
    d->color = Qt::darkYellow;

    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;
    this->path = path;

    this->setTitle(this->algorithm_class);

    this->input_port = new dtkComposerScenePort(dtkComposerScenePort::Input, this);
    this->addInputPort(input_port);
    this->layout();
}

gnomonPipelineNodeWriter::~gnomonPipelineNodeWriter(void)
{

}

//
// gnomonPipelineNodeWriter.cpp ends here