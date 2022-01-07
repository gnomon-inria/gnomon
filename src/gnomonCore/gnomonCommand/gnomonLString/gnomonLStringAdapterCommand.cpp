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

gnomonLStringAdapterCommand::gnomonLStringAdapterCommand() : d(new gnomonLStringAdapterCommandPrivate)
{
    this->factory_name = "lStringAdapter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellImageConstructor::pluginFactory().keys();
    if (!keys.empty()) {
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

        delete this->action;
    this->action = gnomonCore::lStringAdapter::pluginFactory().create(algo_name);
}

void gnomonLStringAdapterCommand::redo()
{
    Q_ASSERT(this->action);
    
    this->action->run();
    
    gnomonAbstractDynamicForm *output = ((gnomonAbstractLStringAdapter *) this->action)->output();
    if ((!output)||(output->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonLStringAdapterCommand::undo()
{
    ((gnomonAbstractLStringAdapter *) this->action)->setInput(nullptr);
}

void gnomonLStringAdapterCommand::setInput(gnomonLStringSeries *input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractLStringAdapter *) this->action)->setInput(d->input);
    }
}

gnomonLStringSeries *gnomonLStringAdapterCommand::input()
{
    return d->input;
}

gnomonAbstractDynamicForm *gnomonLStringAdapterCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonLStringAdapterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonLStringAdapterCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonLStringAdapterCommand::isEmpty()
{
    loadPluginGroup("lStringAdapter");
    return gnomonCore::lStringAdapter::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonLStringAdapterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonLString"));
    return input_types;
}

void gnomonLStringAdapterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonLStringSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}


//
// gnomonLStringAdapterCommand.cpp ends here
