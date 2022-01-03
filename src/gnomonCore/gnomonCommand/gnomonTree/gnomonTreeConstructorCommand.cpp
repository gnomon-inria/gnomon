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

#include "gnomonTreeConstructorCommand.h"

#include <dtkScript>

class gnomonTreeConstructorCommandPrivate
{
public:
    gnomonTreeSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeConstructorCommand::gnomonTreeConstructorCommand() : d(new gnomonTreeConstructorCommandPrivate)
{
    this->factory_name = "treeConstructor";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeConstructor::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeConstructor::pluginFactory().create(this->algorithm_name);
    }
}

gnomonTreeConstructorCommand::~gnomonTreeConstructorCommand()
{
    delete d;
}

void gnomonTreeConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::treeConstructor::pluginFactory().create(algo_name);
}

void gnomonTreeConstructorCommand::redo()
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonTreeSeries *tree = ((gnomonAbstractTreeConstructor *) this->action)->output();
    if ((!tree)||(tree->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = tree;
    }
}

void gnomonTreeConstructorCommand::undo()
{
}

gnomonTreeSeries *gnomonTreeConstructorCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeConstructorCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonTreeConstructorCommand::isEmpty()
{
    loadPluginGroup("treeConstructor");
    return gnomonCore::treeConstructor::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonTreeConstructorCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonTree"));
    return types;
}

//
// gnomonTreeConstructorCommand.cpp ends here
