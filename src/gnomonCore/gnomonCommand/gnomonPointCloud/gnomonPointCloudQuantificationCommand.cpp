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

#include "gnomonPointCloudQuantificationCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonPointCloudQuantificationCommandPrivate
{
public:
    gnomonImageSeries* image = nullptr;
    gnomonPointCloudSeries* pointCloud = nullptr;
};

gnomonPointCloudQuantificationCommand::gnomonPointCloudQuantificationCommand(const QString& key) : d(new gnomonPointCloudQuantificationCommandPrivate)
{
    loadPluginGroup("pointCloudQuantification");

    this->action = gnomonCore::pointCloudQuantification::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonPointCloudQuantificationCommand::~gnomonPointCloudQuantificationCommand(void)
{
    delete d;
}

void gnomonPointCloudQuantificationCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonPointCloudQuantificationCommand::undo(void)
{
    ((gnomonAbstractPointCloudQuantification *) this->action)->setImage(nullptr);
    ((gnomonAbstractPointCloudQuantification *) this->action)->setPointCloud(nullptr);
}

void gnomonPointCloudQuantificationCommand::setImage(gnomonImageSeries *image)
{
    d->image = image;

    Q_ASSERT(this->action);
    ((gnomonAbstractPointCloudQuantification *) this->action)->setImage(d->image);
}


void gnomonPointCloudQuantificationCommand::setPointCloud(gnomonPointCloudSeries *pointCloud)
{
    d->pointCloud = pointCloud;

    Q_ASSERT(this->action);
    ((gnomonAbstractPointCloudQuantification *) this->action)->setPointCloud(d->pointCloud);
}

void gnomonPointCloudQuantificationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonPointCloudQuantificationCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonPointCloudSeries *gnomonPointCloudQuantificationCommand::pointCloud(void)
{
    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudQuantification *) this->action)->pointCloud();
    if ((!pointCloud)||(pointCloud->times().size())==0) {
        return nullptr;
    } else {
        return pointCloud;
    }
}

gnomonDataFrameSeries *gnomonPointCloudQuantificationCommand::dataFrame(void)
{
    gnomonDataFrameSeries *dataFrame = ((gnomonAbstractPointCloudQuantification *) this->action)->dataFrame();
    if ((!dataFrame)||(dataFrame->times().size()==0)) {
        return nullptr;
    }
    else {
        return dataFrame;
    }
}

bool gnomonPointCloudQuantificationCommand::isEmpty(void)
{
    loadPluginGroup("pointCloudQuantification");
    return gnomonCore::pointCloudQuantification::pluginFactory().keys().size() == 0;
}

//
// gnomonPointCloudQuantificationCommand.cpp ends here
