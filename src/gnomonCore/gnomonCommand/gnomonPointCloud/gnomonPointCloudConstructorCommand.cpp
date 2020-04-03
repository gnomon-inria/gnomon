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

#include "gnomonPointCloudConstructorCommand.h"

#include <dtkScript>

class gnomonPointCloudConstructorCommandPrivate
{
public:
    gnomonPointCloudSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonPointCloudConstructorCommand::gnomonPointCloudConstructorCommand(const QString& key) : d(new gnomonPointCloudConstructorCommandPrivate)
{
    this->factory_name = "pointCloudConstructor";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;

    this->action = gnomonCore::pointCloudConstructor::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonPointCloudConstructorCommand::~gnomonPointCloudConstructorCommand(void)
{
    delete d;
}

void gnomonPointCloudConstructorCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudConstructor *) this->action)->output();
    if ((!pointCloud)||(pointCloud->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = pointCloud;
    }
}

void gnomonPointCloudConstructorCommand::undo(void)
{
}

void gnomonPointCloudConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonPointCloudConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonPointCloudSeries *gnomonPointCloudConstructorCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudConstructorCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonPointCloudConstructorCommand::isEmpty(void)
{
    loadPluginGroup("pointCloudConstructor");
    return gnomonCore::pointCloudConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonPointCloudConstructorCommand.cpp ends here
