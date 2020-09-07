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

gnomonMeshAdapterCommand::gnomonMeshAdapterCommand(const QString& key) : d(new gnomonMeshAdapterCommandPrivate)
{
    this->factory_name = "meshAdapter";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::meshAdapter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonMeshAdapterCommand::~gnomonMeshAdapterCommand()
{
    delete d;
}

void gnomonMeshAdapterCommand::redo(void)
{
    Q_ASSERT(this->action);
    
    this->action->run();
    
    gnomonAbstractDynamicForm *output = ((gnomonAbstractMeshAdapter *) this->action)->output();
    if ((!output)||(output->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonMeshAdapterCommand::undo(void)
{
    ((gnomonAbstractMeshAdapter *) this->action)->setInput(nullptr);
}

void gnomonMeshAdapterCommand::setInput(gnomonMeshSeries *input)
{
    if ((!input)||(input->times().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractMeshAdapter *) this->action)->setInput(d->input);
    }
}

gnomonMeshSeries *gnomonMeshAdapterCommand::input(void)
{
    return d->input;
}

gnomonAbstractDynamicForm *gnomonMeshAdapterCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshAdapterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshAdapterCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonMeshAdapterCommand::isEmpty(void)
{
    loadPluginGroup("meshAdapter");
    return gnomonCore::meshAdapter::pluginFactory().keys().size() == 0;
}

//
// gnomonMeshAdapterCommand.cpp ends here
