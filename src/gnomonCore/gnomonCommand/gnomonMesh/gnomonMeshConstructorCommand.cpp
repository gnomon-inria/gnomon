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

#include "gnomonMeshConstructorCommand.h"

#include <dtkScript>

class gnomonMeshConstructorCommandPrivate
{
public:
    gnomonMeshSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshConstructorCommand::gnomonMeshConstructorCommand(const QString& key) : d(new gnomonMeshConstructorCommandPrivate)
{
    loadPluginGroup("meshConstructor");

    this->action = gnomonCore::meshConstructor::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonMeshConstructorCommand::~gnomonMeshConstructorCommand(void)
{
    delete d;
}

void gnomonMeshConstructorCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonMeshConstructorCommand::undo(void)
{
}

void gnomonMeshConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonMeshConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonMeshSeries *gnomonMeshConstructorCommand::output(void)
{
    gnomonMeshSeries *mesh = ((gnomonAbstractMeshConstructor *) this->action)->output();
    if ((!mesh)||(mesh->times().size()==0)) {
        return nullptr;
    } else {
        d->output = mesh;
        return mesh;
    }
}

bool gnomonMeshConstructorCommand::isEmpty(void)
{
    loadPluginGroup("meshConstructor");
    return gnomonCore::meshConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonMeshConstructorCommand.cpp ends here
