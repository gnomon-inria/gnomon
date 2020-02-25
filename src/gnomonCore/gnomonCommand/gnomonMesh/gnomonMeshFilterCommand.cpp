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

#include "gnomonMeshFilterCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonMeshFilterCommandPrivate
{
public:
    gnomonMeshSeries* input = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshFilterCommand::gnomonMeshFilterCommand(const QString& key) : d(new gnomonMeshFilterCommandPrivate)
{
    loadPluginGroup("meshFilter");

    this->action = gnomonCore::meshFilter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonMeshFilterCommand::~gnomonMeshFilterCommand(void)
{
    delete d;
}

void gnomonMeshFilterCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonMeshFilterCommand::undo(void)
{
    ((gnomonAbstractMeshFilter *) this->action)->setInput(nullptr);
}

void gnomonMeshFilterCommand::setInput(gnomonMeshSeries *input)
{
    d->input = input;

    Q_ASSERT(this->action);
    ((gnomonAbstractMeshFilter *) this->action)->setInput(d->input);
}

void gnomonMeshFilterCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonMeshFilterCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonMeshSeries *gnomonMeshFilterCommand::input(void)
{
    gnomonMeshSeries *mesh = ((gnomonAbstractMeshFilter *) this->action)->input();
    if ((!mesh)||(mesh->times().size()==0)) {
        return nullptr;
    } else {
        return mesh;
    }
}

gnomonMeshSeries *gnomonMeshFilterCommand::output(void)
{
    gnomonMeshSeries *mesh = ((gnomonAbstractMeshFilter *) this->action)->output();
    if ((!mesh)||(mesh->times().size()==0)) {
        return nullptr;
    } else {
        return mesh;
    }
}

bool gnomonMeshFilterCommand::isEmpty(void)
{
    loadPluginGroup("meshFilter");
    return gnomonCore::meshFilter::pluginFactory().keys().size() == 0;
}

//
// gnomonMeshFilterCommand.cpp ends here
