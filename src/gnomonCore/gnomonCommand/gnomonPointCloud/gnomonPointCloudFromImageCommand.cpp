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

#include "gnomonPointCloudFromImageCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonPointCloudFromImageCommandPrivate
{
public:
    gnomonImageSeries* input = nullptr;

    gnomonPointCloudSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonPointCloudFromImageCommand::gnomonPointCloudFromImageCommand() : d(new gnomonPointCloudFromImageCommandPrivate)
{
    this->factory_name = "pointCloudFromImage";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::pointCloudFromImage::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::pointCloudFromImage::pluginFactory().create(this->algorithm_name);
    }
}

gnomonPointCloudFromImageCommand::~gnomonPointCloudFromImageCommand()
{
    delete d;
}

void gnomonPointCloudFromImageCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::pointCloudFromImage::pluginFactory().create(algo_name);
}

void gnomonPointCloudFromImageCommand::predo(void)
{

}

void gnomonPointCloudFromImageCommand::postdo(void)
{
    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudFromImage *) this->action)->output();

    if ((!pointCloud)||(pointCloud->times().empty())) {
        d->output = nullptr;
    } else {
        d-> output = pointCloud;
    }
}

void gnomonPointCloudFromImageCommand::undo()
{
    ((gnomonAbstractPointCloudFromImage *) this->action)->setInput(nullptr);
}

void gnomonPointCloudFromImageCommand::setInput(gnomonImageSeries *image)
{
    if ((!image)||(image->times().empty())||(((gnomonImage *)image->current())->channels().empty())) {
        d->input = nullptr;
    } else {
        d->input = image;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractPointCloudFromImage *) this->action)->setInput(d->input);
}

gnomonImageSeries *gnomonPointCloudFromImageCommand::input()
{
    return d->input;
}

gnomonPointCloudSeries *gnomonPointCloudFromImageCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudFromImageCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudFromImageCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonPointCloudFromImageCommand::isEmpty()
{
    loadPluginGroup("pointCloudFromImage");
    return gnomonCore::pointCloudFromImage::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonPointCloudFromImageCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonImage"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonPointCloudFromImageCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonPointCloud"));
    return output_types;
}

void gnomonPointCloudFromImageCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "image") {
        this->setInput(dynamic_cast<gnomonImageSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonPointCloudFromImageCommand.cpp ends here
