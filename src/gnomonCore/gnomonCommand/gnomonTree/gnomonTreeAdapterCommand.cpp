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

#include "gnomonTreeAdapterCommand.h"

#include <dtkScript>

class gnomonTreeAdapterCommandPrivate
{
public:
    gnomonTreeSeries* input = nullptr;
    gnomonAbstractDynamicForm* output = nullptr;
};

gnomonTreeAdapterCommand::gnomonTreeAdapterCommand(const QString& key) : d(new gnomonTreeAdapterCommandPrivate)
{
    this->factory_name = "treeAdapter";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::treeAdapter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonTreeAdapterCommand::~gnomonTreeAdapterCommand()
{
    delete d;
}

void gnomonTreeAdapterCommand::redo(void)
{
    Q_ASSERT(this->action);
    
    this->action->run();
    qDebug()<<Q_FUNC_INFO<<((gnomonAbstractTreeAdapter *) this->action)->output();
    qDebug()<<Q_FUNC_INFO<<((gnomonAbstractTreeAdapter *) this->action)->output()->times();
    qDebug()<<Q_FUNC_INFO<<((gnomonAbstractTreeAdapter *) this->action)->output()->times().size();
    
    gnomonAbstractDynamicForm *output = ((gnomonAbstractTreeAdapter *) this->action)->output();
    if ((!output)||(output->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonTreeAdapterCommand::undo(void)
{
    ((gnomonAbstractTreeAdapter *) this->action)->setInput(nullptr);
}

void gnomonTreeAdapterCommand::setInput(gnomonTreeSeries *input)
{
    if ((!input)||(input->times().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractTreeAdapter *) this->action)->setInput(d->input);
        qDebug()<<Q_FUNC_INFO<<((gnomonAbstractTreeAdapter *) this->action)->input();
    }
}

gnomonTreeSeries *gnomonTreeAdapterCommand::input(void)
{
    return d->input;
}

gnomonAbstractDynamicForm *gnomonTreeAdapterCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeAdapterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeAdapterCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonTreeAdapterCommand::isEmpty(void)
{
    loadPluginGroup("treeAdapter");
    return gnomonCore::treeAdapter::pluginFactory().keys().size() == 0;
}

//
// gnomonTreeAdapterCommand.cpp ends here
