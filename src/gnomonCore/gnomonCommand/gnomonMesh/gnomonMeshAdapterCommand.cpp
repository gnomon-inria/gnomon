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

#include "gnomonMeshAdapterCommand.h"

#include <dtkScript>

class gnomonMeshAdapterCommandPrivate
{
public:
    gnomonMeshSeries* input = nullptr;
    gnomonAbstractDynamicForm* output = nullptr;
};

gnomonMeshAdapterCommand::gnomonMeshAdapterCommand() : d(new gnomonMeshAdapterCommandPrivate)
{
    this->factory_name = "meshAdapter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::meshAdapter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::meshAdapter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonMeshAdapterCommand::~gnomonMeshAdapterCommand()
{
    delete d;
}

void gnomonMeshAdapterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::meshAdapter::pluginFactory().create(algo_name);
}

void gnomonMeshAdapterCommand::redo()
{
    Q_ASSERT(this->action);
    
    this->action->run();
    
    gnomonAbstractDynamicForm *output = ((gnomonAbstractMeshAdapter *) this->action)->output();
    if ((!output)||(output->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonMeshAdapterCommand::undo()
{
    ((gnomonAbstractMeshAdapter *) this->action)->setInput(nullptr);
}

void gnomonMeshAdapterCommand::setInput(gnomonMeshSeries *input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractMeshAdapter *) this->action)->setInput(d->input);
    }
}

gnomonMeshSeries *gnomonMeshAdapterCommand::input()
{
    return d->input;
}

gnomonAbstractDynamicForm *gnomonMeshAdapterCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshAdapterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshAdapterCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonMeshAdapterCommand::isEmpty()
{
    loadPluginGroup("meshAdapter");
    return gnomonCore::meshAdapter::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonMeshAdapterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonMesh"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonMeshAdapterCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonAbstractDynamicForm"));
    return output_types;
}

//
// gnomonMeshAdapterCommand.cpp ends here
