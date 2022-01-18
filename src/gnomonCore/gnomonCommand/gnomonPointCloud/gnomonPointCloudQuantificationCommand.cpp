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

gnomonPointCloudQuantificationCommand::gnomonPointCloudQuantificationCommand() : d(new gnomonPointCloudQuantificationCommandPrivate)
{
    this->factory_name = "pointCloudQuantification";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeFromLString::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeFromLString::pluginFactory().create(this->algorithm_name);
    }
}

gnomonPointCloudQuantificationCommand::~gnomonPointCloudQuantificationCommand()
{
    delete d;
}

void gnomonPointCloudQuantificationCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::pointCloudQuantification::pluginFactory().create(algo_name);
}

void gnomonPointCloudQuantificationCommand::predo(void)
{

}

void gnomonPointCloudQuantificationCommand::postdo(void)
{
    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudQuantification *) this->action)->pointCloud();

    if ((!pointCloud)||pointCloud->times().empty()) {
        d->pointCloud = nullptr;
    } else {
        d->pointCloud = pointCloud;
    }

    gnomonDataFrameSeries *dataFrame = ((gnomonAbstractPointCloudQuantification *) this->action)->dataFrame();

    if ((!dataFrame)||(dataFrame->times().empty())) {
        d->dataFrame = nullptr;
    }
    else {
        d->dataFrame = dataFrame;
    }
}

void gnomonPointCloudQuantificationCommand::undo()
{
    ((gnomonAbstractPointCloudQuantification *) this->action)->setImage(nullptr);
    ((gnomonAbstractPointCloudQuantification *) this->action)->setPointCloud(nullptr);
}

void gnomonPointCloudQuantificationCommand::setImage(gnomonImageSeries *image)
{
    if ((!image)||(image->times().empty())||(((gnomonImage *)image->current())->channels().empty())) {
        d->image = nullptr;
    } else {
        d->image = image;
        Q_ASSERT(this->action);
        ((gnomonAbstractPointCloudQuantification *) this->action)->setImage(d->image);
    }
}

void gnomonPointCloudQuantificationCommand::setPointCloud(gnomonPointCloudSeries *pointCloud)
{
    if ((!pointCloud)||(pointCloud->times().empty())) {
        d->input_pointCloud = nullptr;
    } else {
        d->input_pointCloud = pointCloud;

        Q_ASSERT(this->action);
        ((gnomonAbstractPointCloudQuantification *) this->action)->setPointCloud(d->input_pointCloud);
    }
}

gnomonPointCloudSeries *gnomonPointCloudQuantificationCommand::pointCloud()
{
    return d->pointCloud;
}

gnomonDataFrameSeries *gnomonPointCloudQuantificationCommand::dataFrame()
{
    return d->dataFrame;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudQuantificationCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["image"] = d->image;
    inputs["pointCloud"] = d->input_pointCloud;
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudQuantificationCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["pointCloud"] = this->pointCloud();
    outputs["dataFrame"] = this->dataFrame();
    return outputs;
}

bool gnomonPointCloudQuantificationCommand::isEmpty()
{
    loadPluginGroup("pointCloudQuantification");
    return gnomonCore::pointCloudQuantification::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonPointCloudQuantificationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("image", "gnomonImage"));
    input_types.emplace_back(std::make_pair("pointCloud", "gnomonPointCloud"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonPointCloudQuantificationCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("pointCloud", "gnomonPointCloud"));
    output_types.emplace_back(std::make_pair("dataFrame", "gnomonDataFrame"));
    return output_types;
}

void gnomonPointCloudQuantificationCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "image") {
        this->setImage(dynamic_cast<gnomonImageSeries *>(form));
    } else if (name == "pointCloud") {
        this->setPointCloud(dynamic_cast<gnomonPointCloudSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonPointCloudQuantificationCommand.cpp ends here
