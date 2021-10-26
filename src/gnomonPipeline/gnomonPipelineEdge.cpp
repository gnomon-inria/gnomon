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

#include "gnomonPipelineEdge.h"

#include "gnomonPipelineNode.h"
#include "gnomonPipelinePort.h"

class gnomonPipelineEdgePrivate
{
public:
    gnomonPipelineNode *parent;

public:
    gnomonPipelinePort *source;
    gnomonPipelinePort *target;
};

gnomonPipelineEdge::gnomonPipelineEdge(void) : d(new gnomonPipelineEdgePrivate)
{
    d->source = nullptr;
    d->target = nullptr;

    d->parent = nullptr;
}

gnomonPipelineEdge::~gnomonPipelineEdge(void)
{
    delete d;

    d = nullptr;
}


gnomonPipelinePort *gnomonPipelineEdge::source(void)
{
    return d->source;
}

gnomonPipelinePort *gnomonPipelineEdge::target(void)
{
    return d->target;
}

void gnomonPipelineEdge::setSource(gnomonPipelinePort *port)
{
    d->source = port;
}

void gnomonPipelineEdge::setTarget(gnomonPipelinePort *port)
{
    d->target = port;
}

bool gnomonPipelineEdge::link(void)
{
    if (!d->source || !d->target)
        return false;

    if (d->source == d->target)
        return false;

    if (d->source->node() == d->target->node())
        return false;

    if (d->source->type() == gnomonPipelinePort::Output)
        d->source->node()->addOutputEdge(this);

    if (d->target->type() == gnomonPipelinePort::Input)
        d->target->node()->addInputEdge(this);

    return true;
}

bool gnomonPipelineEdge::unlink(void)
{
    if (!d->source || !d->target)
        return false;

    if (d->source->type() == gnomonPipelinePort::Output)
        d->source->node()->removeOutputEdge(this);

    if (d->target->type() == gnomonPipelinePort::Input)
        d->target->node()->removeInputEdge(this);

    return true;
}

gnomonPipelineNode *gnomonPipelineEdge::parent(void)
{
    return d->parent;
}

void gnomonPipelineEdge::setParent(gnomonPipelineNode *parent)
{
    d->parent = parent;
}



//
// gnomonPipelineEdge.cpp ends here