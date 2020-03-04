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
    loadPluginGroup("pointCloudConstructor");

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
    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudConstructor *) this->action)->output();
    if ((!pointCloud)||(pointCloud->times().size()==0)) {
        return nullptr;
    } else {
        d->output = pointCloud;
        return pointCloud;
    }
}

bool gnomonPointCloudConstructorCommand::isEmpty(void)
{
    loadPluginGroup("pointCloudConstructor");
    return gnomonCore::pointCloudConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonPointCloudConstructorCommand.cpp ends here
