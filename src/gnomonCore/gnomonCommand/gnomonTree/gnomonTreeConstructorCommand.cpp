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

gnomonTreeConstructorCommand::gnomonTreeConstructorCommand(void) : d(new gnomonTreeConstructorCommandPrivate)
{
    this->factory_name = "treeConstructor";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeConstructor::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeConstructor::pluginFactory().create(this->algorithm_name);
    }
}

gnomonTreeConstructorCommand::~gnomonTreeConstructorCommand(void)
{
    delete d;
}

void gnomonTreeConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::treeConstructor::pluginFactory().create(algo_name);
}

void gnomonTreeConstructorCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonTreeSeries *tree = ((gnomonAbstractTreeConstructor *) this->action)->output();
    if ((!tree)||(tree->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = tree;
    }
}

void gnomonTreeConstructorCommand::undo(void)
{
}

void gnomonTreeConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

dtkCoreParameters gnomonTreeConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonTreeSeries *gnomonTreeConstructorCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeConstructorCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonTreeConstructorCommand::isEmpty(void)
{
    loadPluginGroup("treeConstructor");
    return gnomonCore::treeConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonTreeConstructorCommand.cpp ends here
