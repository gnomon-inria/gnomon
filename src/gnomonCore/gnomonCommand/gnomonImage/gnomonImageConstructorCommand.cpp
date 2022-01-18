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

#include "gnomonImageConstructorCommand.h"

#include <dtkScript>

class gnomonImageConstructorCommandPrivate
{
public:
    gnomonImageSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageConstructorCommand::gnomonImageConstructorCommand() : d(new gnomonImageConstructorCommandPrivate)
{
    this->factory_name = "imageConstructor";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::imageConstructor::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::imageConstructor::pluginFactory().create(this->algorithm_name);
    }

}

gnomonImageConstructorCommand::~gnomonImageConstructorCommand()
{
    delete d;
}

void gnomonImageConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::imageConstructor::pluginFactory().create(algo_name);
}

void gnomonImageConstructorCommand::predo(void)
{

}

void gnomonImageConstructorCommand::postdo(void)
{
    gnomonImageSeries *image = ((gnomonAbstractImageConstructor *) this->action)->output();

    if ((!image)||(image->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonImageConstructorCommand::undo()
{
}

gnomonImageSeries *gnomonImageConstructorCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageConstructorCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonImageConstructorCommand::isEmpty()
{
    loadPluginGroup("imageConstructor");
    return gnomonCore::imageConstructor::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonImageConstructorCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonImage"));
    return types;
}

//
// gnomonImageConstructorCommand.cpp ends here
