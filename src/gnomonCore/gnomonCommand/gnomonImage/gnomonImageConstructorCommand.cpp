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

gnomonImageConstructorCommand::gnomonImageConstructorCommand(const QString& key) : d(new gnomonImageConstructorCommandPrivate)
{
    this->factory_name = "imageConstructor";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;

    this->action = gnomonCore::imageConstructor::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImageConstructorCommand::~gnomonImageConstructorCommand(void)
{
    delete d;
}

void gnomonImageConstructorCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonImageSeries *image = ((gnomonAbstractImageConstructor *) this->action)->output();
    if ((!image)||(image->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonImageConstructorCommand::undo(void)
{
}

void gnomonImageConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

dtkCoreParameters gnomonImageConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImageSeries *gnomonImageConstructorCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageConstructorCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonImageConstructorCommand::isEmpty(void)
{
    loadPluginGroup("imageConstructor");
    return gnomonCore::imageConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonImageConstructorCommand.cpp ends here
