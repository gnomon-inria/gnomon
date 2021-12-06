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

#include "gnomonPipelineNode.h"
#include "gnomonPipelinePort.h"

class gnomonPipelinePortPrivate
{
public:
    gnomonPipelinePort::Type type;

public:
    gnomonPipelineNode *node;

public:
    QString label;
};

gnomonPipelinePort::gnomonPipelinePort(Type type, gnomonPipelineNode *parent) :  d(new gnomonPipelinePortPrivate)
{
    d->type = type;
    d->node = parent;
}

gnomonPipelinePort::gnomonPipelinePort(Type type, const QString& label, gnomonPipelineNode *parent) : d(new gnomonPipelinePortPrivate)
{
    d->type = type;
    d->node = parent;
    this->setLabel(label);
}

gnomonPipelinePort::~gnomonPipelinePort(void)
{
    delete d;
    d = nullptr;
}

gnomonPipelinePort::Type gnomonPipelinePort::type(void)
{
    return d->type;
}

gnomonPipelineNode *gnomonPipelinePort::node(void)
{
    return d->node;
}

QString gnomonPipelinePort::label(void)
{
    return d->label;
}

void gnomonPipelinePort::setLabel(const QString& label)
{
    d->label = label;
}

//
// gnomonPipelinePort.cpp ends here