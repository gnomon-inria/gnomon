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

#include "gnomonCellComplexAdapterCommand.h"

#include <dtkScript>

class gnomonCellComplexAdapterCommandPrivate
{
public:
    gnomonCellComplexSeries* input = nullptr;
    gnomonAbstractDynamicForm* output = nullptr;
};

gnomonCellComplexAdapterCommand::gnomonCellComplexAdapterCommand(void) : d(new gnomonCellComplexAdapterCommandPrivate)
{
    this->factory_name = "cellComplexAdapter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellComplexAdapter::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellComplexAdapter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellComplexAdapterCommand::~gnomonCellComplexAdapterCommand()
{
    delete d;
}

void gnomonCellComplexAdapterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::cellComplexAdapter::pluginFactory().create(algo_name);
}

void gnomonCellComplexAdapterCommand::redo(void)
{
    Q_ASSERT(this->action);
    
    this->action->run();
    
    gnomonAbstractDynamicForm *output = ((gnomonAbstractCellComplexAdapter *) this->action)->output();
    if ((!output)||(output->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonCellComplexAdapterCommand::undo(void)
{
    ((gnomonAbstractCellComplexAdapter *) this->action)->setInput(nullptr);
}

void gnomonCellComplexAdapterCommand::setInput(gnomonCellComplexSeries *input)
{
    if ((!input)||(input->times().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellComplexAdapter *) this->action)->setInput(d->input);
    }
}

gnomonCellComplexSeries *gnomonCellComplexAdapterCommand::input(void)
{
    return d->input;
}

gnomonAbstractDynamicForm *gnomonCellComplexAdapterCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexAdapterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexAdapterCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonCellComplexAdapterCommand::isEmpty(void)
{
    loadPluginGroup("cellComplexAdapter");
    return gnomonCore::cellComplexAdapter::pluginFactory().keys().size() == 0;
}

//
// gnomonCellComplexAdapterCommand.cpp ends here
