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

gnomonPointCloudConstructorCommand::gnomonPointCloudConstructorCommand(void) : d(new gnomonPointCloudConstructorCommandPrivate)
{
    this->factory_name = "pointCloudConstructor";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeFromLString::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeFromLString::pluginFactory().create(this->algorithm_name);
    }
}

gnomonPointCloudConstructorCommand::~gnomonPointCloudConstructorCommand(void)
{
    delete d;
}

void gnomonPointCloudConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::pointCloudConstructor::pluginFactory().create(algo_name);
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

dtkCoreParameters gnomonPointCloudConstructorCommand::parameters(void) const
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
