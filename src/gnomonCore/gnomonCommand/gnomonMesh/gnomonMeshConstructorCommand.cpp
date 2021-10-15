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

gnomonMeshConstructorCommand::gnomonMeshConstructorCommand(void) : d(new gnomonMeshConstructorCommandPrivate)
{
    this->factory_name = "meshConstructor";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::meshConstructor::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::meshConstructor::pluginFactory().create(this->algorithm_name);
    }

}

gnomonMeshConstructorCommand::~gnomonMeshConstructorCommand(void)
{
    delete d;
}

void gnomonMeshConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::meshConstructor::pluginFactory().create(algo_name);
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

dtkCoreParameters gnomonMeshConstructorCommand::parameters(void) const
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
