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

#include "gnomonCellImageConstructorCommand.h"

#include <dtkScript>

class gnomonCellImageConstructorCommandPrivate
{
public:
    gnomonCellImageSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageConstructorCommand::gnomonCellImageConstructorCommand() : d(new gnomonCellImageConstructorCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellImageConstructor::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellImageConstructor::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellImageConstructorCommand::~gnomonCellImageConstructorCommand()
{
    delete d;
}

void gnomonCellImageConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellImageConstructor::pluginFactory().create(algo_name);
}

void gnomonCellImageConstructorCommand::redo()
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageConstructor *) this->action)->output();
    if ((!cellImage)||(cellImage->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = cellImage;
    }
}

void gnomonCellImageConstructorCommand::undo()
{
}

gnomonCellImageSeries *gnomonCellImageConstructorCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageConstructorCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonCellImageConstructorCommand::isEmpty()
{
    return availablePlugins().empty();
}

gnomonAbstractCommand::orderedMap gnomonCellImageConstructorCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonCellImage"));
    return types;
}

QStringList gnomonCellImageConstructorCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

//
// gnomonCellImageConstructorCommand.cpp ends here
