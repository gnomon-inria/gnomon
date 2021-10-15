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
    gnomonPointCloudSeries* input_pointCloud = nullptr;

    gnomonPointCloudSeries* pointCloud = nullptr;
    gnomonDataFrameSeries* dataFrame = nullptr;
};

gnomonPointCloudQuantificationCommand::gnomonPointCloudQuantificationCommand(void) : d(new gnomonPointCloudQuantificationCommandPrivate)
{
    this->factory_name = "pointCloudQuantification";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeFromLString::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeFromLString::pluginFactory().create(this->algorithm_name);
    }
}

gnomonPointCloudQuantificationCommand::~gnomonPointCloudQuantificationCommand(void)
{
    delete d;
}

void gnomonPointCloudQuantificationCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::pointCloudQuantification::pluginFactory().create(algo_name);
}

void gnomonPointCloudQuantificationCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudQuantification *) this->action)->pointCloud();
    if ((!pointCloud)||(pointCloud->times().size())==0) {
        d->pointCloud = nullptr;
    } else {
        d->pointCloud = pointCloud;
    }

    gnomonDataFrameSeries *dataFrame = ((gnomonAbstractPointCloudQuantification *) this->action)->dataFrame();
    if ((!dataFrame)||(dataFrame->times().size()==0)) {
        d->dataFrame = nullptr;
    }
    else {
        d->dataFrame = dataFrame;
    }
}

void gnomonPointCloudQuantificationCommand::undo(void)
{
    ((gnomonAbstractPointCloudQuantification *) this->action)->setImage(nullptr);
    ((gnomonAbstractPointCloudQuantification *) this->action)->setPointCloud(nullptr);
}

void gnomonPointCloudQuantificationCommand::setImage(gnomonImageSeries *image)
{
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        d->image = nullptr;
    } else {
        d->image = image;
        Q_ASSERT(this->action);
        ((gnomonAbstractPointCloudQuantification *) this->action)->setImage(d->image);
    }
}

void gnomonPointCloudQuantificationCommand::setPointCloud(gnomonPointCloudSeries *pointCloud)
{
    if ((!pointCloud)||(pointCloud->times().size()==0)) {
        d->input_pointCloud = nullptr;
    } else {
        d->input_pointCloud = pointCloud;

        Q_ASSERT(this->action);
        ((gnomonAbstractPointCloudQuantification *) this->action)->setPointCloud(d->input_pointCloud);
    }
}

void gnomonPointCloudQuantificationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

dtkCoreParameters gnomonPointCloudQuantificationCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonPointCloudSeries *gnomonPointCloudQuantificationCommand::pointCloud(void)
{
    return d->pointCloud;
}

gnomonDataFrameSeries *gnomonPointCloudQuantificationCommand::dataFrame(void)
{
    return d->dataFrame;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudQuantificationCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["image"] = d->image;
    inputs["pointCloud"] = d->input_pointCloud;
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudQuantificationCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["pointCloud"] = this->pointCloud();
    outputs["dataFrame"] = this->dataFrame();
    return outputs;
}

bool gnomonPointCloudQuantificationCommand::isEmpty(void)
{
    loadPluginGroup("pointCloudQuantification");
    return gnomonCore::pointCloudQuantification::pluginFactory().keys().size() == 0;
}

//
// gnomonPointCloudQuantificationCommand.cpp ends here
