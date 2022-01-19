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

#include "gnomonCellComplexConstructorCommand.h"

#include <dtkScript>

class gnomonCellComplexConstructorCommandPrivate
{
public:
    gnomonCellComplexSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellComplexConstructorCommand::gnomonCellComplexConstructorCommand() : d(new gnomonCellComplexConstructorCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellComplexConstructor::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellComplexConstructor::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellComplexConstructorCommand::~gnomonCellComplexConstructorCommand()
{
    delete d;
}

void gnomonCellComplexConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellComplexConstructor::pluginFactory().create(algo_name);
}


void gnomonCellComplexConstructorCommand::redo()
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellComplexSeries *cellComplex = ((gnomonAbstractCellComplexConstructor *) this->action)->output();
    if ((!cellComplex)||(cellComplex->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = cellComplex;
    }
}

void gnomonCellComplexConstructorCommand::undo()
{
}

gnomonCellComplexSeries *gnomonCellComplexConstructorCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexConstructorCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonCellComplexConstructorCommand::isEmpty()
{
    return availablePlugins().empty();
}

gnomonAbstractCommand::orderedMap gnomonCellComplexConstructorCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonCellComplex"));
    return types;
}

QStringList gnomonCellComplexConstructorCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

//
// gnomonCellComplexConstructorCommand.cpp ends here
