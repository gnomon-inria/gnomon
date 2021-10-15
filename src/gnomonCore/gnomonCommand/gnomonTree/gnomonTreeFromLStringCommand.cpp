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

#include "gnomonTreeFromLStringCommand.h"

#include <dtkScript>

class gnomonTreeFromLStringCommandPrivate
{
public:
    gnomonLStringSeries *input = nullptr;
    gnomonTreeSeries *output = nullptr;
};

gnomonTreeFromLStringCommand::gnomonTreeFromLStringCommand(void) : d(new gnomonTreeFromLStringCommandPrivate)
{
    this->factory_name = "treeFromLString";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeFromLString::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeFromLString::pluginFactory().create(this->algorithm_name);
    }
}

gnomonTreeFromLStringCommand::~gnomonTreeFromLStringCommand(void)
{
    delete d;
}

void gnomonTreeFromLStringCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::treeFromLString::pluginFactory().create(algo_name);
}


void gnomonTreeFromLStringCommand::redo(void)
{
    Q_ASSERT(this->action);
//    ((gnomonAbstractTreeFromLString *) this->action)->setLSystem(d->lsystem);
    this->action->run();

    gnomonTreeSeries *tree = ((gnomonAbstractTreeFromLString *) this->action)->output();
    if ((!tree)||(tree->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = tree;
    }
}

void gnomonTreeFromLStringCommand::undo(void)
{
    ((gnomonAbstractTreeFromLString *) this->action)->setInput(nullptr);
}

//void gnomonTreeFromLStringCommand::setLSystem(const QString& lsystem)
//{
//    d->lsystem = lsystem;
//}

void gnomonTreeFromLStringCommand::setInput(gnomonLStringSeries *input)
{
    if ((!input)||(input->times().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractTreeFromLString *) this->action)->setInput(d->input);
    }
}

gnomonLStringSeries *gnomonTreeFromLStringCommand::input(void)
{
    return d->input;
}

gnomonTreeSeries *gnomonTreeFromLStringCommand::output(void)
{
    return d->output;
}

dtkCoreParameters gnomonTreeFromLStringCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonTreeFromLStringCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeFromLStringCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeFromLStringCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonTreeFromLStringCommand::isEmpty(void)
{
    loadPluginGroup("treeFromLString");
    return gnomonCore::treeFromLString::pluginFactory().keys().size() == 0;
}

//
// gnomonTreeFromLStringCommand.cpp ends here
