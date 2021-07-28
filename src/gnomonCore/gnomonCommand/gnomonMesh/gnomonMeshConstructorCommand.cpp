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
    this->factory_name = "meshConstructor";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;

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

    gnomonMeshSeries *mesh = ((gnomonAbstractMeshConstructor *) this->action)->output();
    if ((!mesh)||(mesh->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = mesh;
    }
}

void gnomonMeshConstructorCommand::undo(void)
{
}

void gnomonMeshConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, dtkCoreParameter *> gnomonMeshConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonMeshSeries *gnomonMeshConstructorCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshConstructorCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonMeshConstructorCommand::isEmpty(void)
{
    loadPluginGroup("meshConstructor");
    return gnomonCore::meshConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonMeshConstructorCommand.cpp ends here
