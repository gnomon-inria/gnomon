#include "gnomonCellImageFromImageCommand.h"

#include <dtkLog>
#include <dtkScript>
#include <dtkImagingCore>

class gnomonCellImageFromImageCommandPrivate
{
public:
    gnomonImageSeries *image_series = nullptr;
    gnomonPointCloudSeries *pointCloud_series = nullptr;
    gnomonBinaryImageSeries *binary_image_series = nullptr;

    gnomonCellImageSeries *output = nullptr;
};

gnomonCellImageFromImageCommand::gnomonCellImageFromImageCommand() : d(new gnomonCellImageFromImageCommandPrivate)
{
    this->factory_name = "cellImageFromImage";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellImageFromImage::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellImageFromImage::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellImageFromImageCommand::~gnomonCellImageFromImageCommand()
{
    delete d;
}

void gnomonCellImageFromImageCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellImageFromImage::pluginFactory().create(algo_name);
}

void gnomonCellImageFromImageCommand::redo()
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageFromImage *) this->action)->output();
    if ((!cellImage)||cellImage->times().empty()) {
        d->output = nullptr;
    } else {
        d->output = cellImage;
    }
}

void gnomonCellImageFromImageCommand::undo()
{
    Q_ASSERT(this->action);

    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(nullptr);
    ((gnomonAbstractCellImageFromImage *) this->action)->setCellPoints(nullptr);
}

void gnomonCellImageFromImageCommand::setInput(gnomonImageSeries* image_series)
{
    if ((!image_series)||(image_series->times().empty())||(((gnomonImage *)image_series->current())->channels().empty())) {
        d->image_series = nullptr;
    } else {
        d->image_series = image_series;
    }
    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(d->image_series);
}

gnomonImageSeries *gnomonCellImageFromImageCommand::input()
{
    return d->image_series;
}

void gnomonCellImageFromImageCommand::setCellPoints(gnomonPointCloudSeries *pointCloud_series)
{
    if ((!pointCloud_series)||(pointCloud_series->times().empty())) {
        d->pointCloud_series = nullptr;
    } else {
        d->pointCloud_series = pointCloud_series;
    }
    ((gnomonAbstractCellImageFromImage *) this->action)->setCellPoints(d->pointCloud_series);
}

gnomonPointCloudSeries *gnomonCellImageFromImageCommand::cellPoints()
{
    return d->pointCloud_series;
}

gnomonCellImageSeries *gnomonCellImageFromImageCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageFromImageCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    inputs["cellPoints"] = this->cellPoints();
    inputs["binaryImage"] = this->binaryImage();
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonCellImageFromImageCommand::inputTypes()
{
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonImage"));
    input_types.emplace_back(std::make_pair("cellPoints", "gnomonPointCloud"));
    input_types.emplace_back(std::make_pair("binaryImage", "gnomonBinaryImage"));
    return input_types;
}

void gnomonCellImageFromImageCommand::setInputForm(const QString& name, gnomonAbstractDynamicForm *form)
{
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonImageSeries *>(form));
    } else if (name == "cellPoints") {
        this->setCellPoints(dynamic_cast<gnomonPointCloudSeries *>(form));
    } else if (name == "binaryImage") {
        this->setBinaryImage(dynamic_cast<gnomonBinaryImageSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageFromImageCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonCellImageFromImageCommand::outputTypes()
{
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonCellImage"));
    return output_types;
}

bool gnomonCellImageFromImageCommand::isEmpty()
{
    loadPluginGroup("cellImageFromImage");
    return gnomonCore::cellImageFromImage::pluginFactory().keys().empty();
}

void gnomonCellImageFromImageCommand::setBinaryImage(gnomonBinaryImageSeries *binary_image_series) {
    if ((!binary_image_series)||(binary_image_series->times().empty())) {
        d->binary_image_series = nullptr;
    } else {
        d->binary_image_series = binary_image_series;
    }
    ((gnomonAbstractCellImageFromImage *) this->action)->setBinaryImage(d->binary_image_series);
}

gnomonBinaryImageSeries *gnomonCellImageFromImageCommand::binaryImage() {
    return d->binary_image_series;
}

//
// gnomonCellImageFromImageCommand.cpp ends here
