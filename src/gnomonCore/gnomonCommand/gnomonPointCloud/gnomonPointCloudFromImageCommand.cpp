#include "gnomonPointCloudFromImageCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudFromImage.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonPointCloudFromImageCommandPrivate
{
public:
    std::shared_ptr<gnomonImageSeries> input;
    std::shared_ptr<gnomonCellImageSeries> cellImage;
    std::shared_ptr<gnomonPointCloudSeries> output;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonPointCloudFromImageCommand::gnomonPointCloudFromImageCommand() : d(new gnomonPointCloudFromImageCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonPointCloudFromImageCommand)

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
    this->action->is_async = true;
}

void gnomonPointCloudFromImageCommand::postdo(void)
{
    std::shared_ptr<gnomonPointCloudSeries> pointCloud = ((gnomonAbstractPointCloudFromImage *) this->action)->output();

    if ((!pointCloud)||(pointCloud->times().empty())) {
        d->output = nullptr;
    } else {
        d-> output = pointCloud;
    }
}

void gnomonPointCloudFromImageCommand::undo()
{
    ((gnomonAbstractPointCloudFromImage *) this->action)->setInput(nullptr);
    this->action->refreshParameters();
}

void gnomonPointCloudFromImageCommand::setInput(std::shared_ptr<gnomonImageSeries> image)
{
    if ((!image)||(image->times().empty())||(image->current()->channels().empty())) {
        d->input = nullptr;
    } else {
        d->input = image;
    }
    Q_ASSERT(this->action);
    if (d->input) {
        d->input->load();
    }
    ((gnomonAbstractPointCloudFromImage *) this->action)->setInput(d->input);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonImageSeries> gnomonPointCloudFromImageCommand::input()
{
    return d->input;
}

void gnomonPointCloudFromImageCommand::setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage)
{
    if ((!cellImage)||(cellImage->times().empty())) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }
    Q_ASSERT(this->action);
    if (d->cellImage) {
        d->cellImage->load();
    }
    ((gnomonAbstractPointCloudFromImage *) this->action)->setCellImage(d->cellImage);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonCellImageSeries> gnomonPointCloudFromImageCommand::cellImage()
{
    return d->cellImage;
}

std::shared_ptr<gnomonPointCloudSeries> gnomonPointCloudFromImageCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonPointCloudFromImageCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["input"] = this->input();
    inputs["cellImage"] = this->cellImage();
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonPointCloudFromImageCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonPointCloudFromImageCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonImage"));
    input_types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonPointCloudFromImageCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonPointCloud"));
    return output_types;
}

void gnomonPointCloudFromImageCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else if (name == "cellImage") {
        this->setCellImage(std::dynamic_pointer_cast<gnomonCellImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonPointCloudFromImageCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonPointCloudSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonPointCloudFromImageCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

void gnomonPointCloudFromImageCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->output = nullptr;
    d->input = nullptr;
    d->cellImage = nullptr;
}

//
// gnomonPointCloudFromImageCommand.cpp ends here
