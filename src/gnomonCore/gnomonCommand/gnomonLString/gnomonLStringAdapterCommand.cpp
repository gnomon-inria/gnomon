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

#include "gnomonLStringAdapterCommand.h"

#include <dtkScript>

class gnomonLStringAdapterCommandPrivate
{
public:
    gnomonLStringSeries* input = nullptr;
    gnomonAbstractDynamicForm* output = nullptr;
};

gnomonLStringAdapterCommand::gnomonLStringAdapterCommand(void) : d(new gnomonLStringAdapterCommandPrivate)
{
    this->factory_name = "lStringAdapter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellImageConstructor::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellImageConstructor::pluginFactory().create(this->algorithm_name);
    }
}

gnomonLStringAdapterCommand::~gnomonLStringAdapterCommand()
{
    delete d;
}

void gnomonLStringAdapterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::lStringAdapter::pluginFactory().create(algo_name);
}

void gnomonLStringAdapterCommand::redo(void)
{
    Q_ASSERT(this->action);
    
    this->action->run();
    
    gnomonAbstractDynamicForm *output = ((gnomonAbstractLStringAdapter *) this->action)->output();
    if ((!output)||(output->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonLStringAdapterCommand::undo(void)
{
    ((gnomonAbstractLStringAdapter *) this->action)->setInput(nullptr);
}

void gnomonLStringAdapterCommand::setInput(gnomonLStringSeries *input)
{
    if ((!input)||(input->times().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractLStringAdapter *) this->action)->setInput(d->input);
    }
}

gnomonLStringSeries *gnomonLStringAdapterCommand::input(void)
{
    return d->input;
}

gnomonAbstractDynamicForm *gnomonLStringAdapterCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonLStringAdapterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonLStringAdapterCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonLStringAdapterCommand::isEmpty(void)
{
    loadPluginGroup("lStringAdapter");
    return gnomonCore::lStringAdapter::pluginFactory().keys().size() == 0;
}

//
// gnomonLStringAdapterCommand.cpp ends here
