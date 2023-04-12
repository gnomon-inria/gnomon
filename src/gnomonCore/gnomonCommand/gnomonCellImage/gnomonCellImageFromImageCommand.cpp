#include "gnomonCellImageFromImageCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageFromImage.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <dtkLog>

class gnomonCellImageFromImageCommandPrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image_series = nullptr;
    std::shared_ptr<gnomonPointCloudSeries> pointCloud_series = nullptr;
    std::shared_ptr<gnomonBinaryImageSeries> binary_image_series = nullptr;

    std::shared_ptr<gnomonCellImageSeries> output = nullptr;
};

gnomonCellImageFromImageCommand::gnomonCellImageFromImageCommand() : d(new gnomonCellImageFromImageCommandPrivate)
{
    this->factory_name = groupName;
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

void gnomonCellImageFromImageCommand::predo(void)
{
    this->action->is_async = true;
}

void gnomonCellImageFromImageCommand::postdo(void)
{
    std::shared_ptr<gnomonCellImageSeries> cellImage = ((gnomonAbstractCellImageFromImage *) this->action)->output();

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

void gnomonCellImageFromImageCommand::setInput(std::shared_ptr<gnomonImageSeries> image_series)
{
    if ((!image_series)||(image_series->times().empty())||(image_series->current()->channels().empty())) {
        d->image_series = nullptr;
    } else {
        d->image_series = image_series;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(d->image_series);
    ((gnomonAbstractCellImageFromImage *) this->action)->refreshParameters();
}

std::shared_ptr<gnomonImageSeries> gnomonCellImageFromImageCommand::input()
{
    return d->image_series;
}

void gnomonCellImageFromImageCommand::setCellPoints(std::shared_ptr<gnomonPointCloudSeries> pointCloud_series)
{
    if ((!pointCloud_series)||(pointCloud_series->times().empty())) {
        d->pointCloud_series = nullptr;
    } else {
        d->pointCloud_series = pointCloud_series;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageFromImage *) this->action)->setCellPoints(d->pointCloud_series);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonPointCloudSeries> gnomonCellImageFromImageCommand::cellPoints()
{
    return d->pointCloud_series;
}

std::shared_ptr<gnomonCellImageSeries> gnomonCellImageFromImageCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageFromImageCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
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

void gnomonCellImageFromImageCommand::setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else if (name == "cellPoints") {
        this->setCellPoints(std::dynamic_pointer_cast<gnomonPointCloudSeries>(form));
    } else if (name == "binaryImage") {
        this->setBinaryImage(std::dynamic_pointer_cast<gnomonBinaryImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageFromImageCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
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
    return availablePlugins().empty();
}

void gnomonCellImageFromImageCommand::setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> binary_image_series) {
    if ((!binary_image_series)||(binary_image_series->times().empty())) {
        d->binary_image_series = nullptr;
    } else {
        d->binary_image_series = binary_image_series;
    }
     Q_ASSERT(this->action);
    ((gnomonAbstractCellImageFromImage *) this->action)->setBinaryImage(d->binary_image_series);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonBinaryImageSeries> gnomonCellImageFromImageCommand::binaryImage() {
    return d->binary_image_series;
}

QStringList gnomonCellImageFromImageCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

void gnomonCellImageFromImageCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonCellImageSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonCellImageFromImageCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

//
// gnomonCellImageFromImageCommand.cpp ends here
