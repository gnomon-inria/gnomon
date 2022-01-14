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

gnomonMeshConstructorCommand::gnomonMeshConstructorCommand() : d(new gnomonMeshConstructorCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::meshConstructor::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::meshConstructor::pluginFactory().create(this->algorithm_name);
    }

}

gnomonMeshConstructorCommand::~gnomonMeshConstructorCommand()
{
    delete d;
}

void gnomonMeshConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::meshConstructor::pluginFactory().create(algo_name);
}

void gnomonMeshConstructorCommand::redo()
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonMeshSeries *mesh = ((gnomonAbstractMeshConstructor *) this->action)->output();
    if ((!mesh)||(mesh->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = mesh;
    }
}

void gnomonMeshConstructorCommand::undo()
{
}

gnomonMeshSeries *gnomonMeshConstructorCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshConstructorCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonMeshConstructorCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonMeshConstructorCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonMeshConstructorCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonMesh"));
    return types;
}

//
// gnomonMeshConstructorCommand.cpp ends here
