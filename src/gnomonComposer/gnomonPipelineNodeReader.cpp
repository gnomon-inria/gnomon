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

#include "gnomonPipelineNodeReader.h"

#include "gnomonComposerSceneNode_p.h"

// /////////////////////////////////////////////////////////////////
// gnomonPipelineNodeReader
// /////////////////////////////////////////////////////////////////

gnomonPipelineNodeReader::gnomonPipelineNodeReader(const QString& algorithm_class, const QString& algorithm, const QString& path) : gnomonComposerSceneNode()
{
    d->color = QColor(170, 255, 0);

    this->algorithm_class = algorithm_class;
    this->algorithm = algorithm;
    this->path = path;

    this->setTitle(this->algorithm_class);

    this->output_port = new dtkComposerScenePort(dtkComposerScenePort::Output, this);
    this->addOutputPort(output_port);
    this->layout();
}

gnomonPipelineNodeReader::~gnomonPipelineNodeReader(void)
{

}

//
// gnomonPipelineNodeReader.cpp ends here