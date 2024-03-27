#include "gnomonMeshFromImageCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshFromImage.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonMeshFromImageCommandPrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image = nullptr;
    std::shared_ptr<gnomonCellImageSeries> cellImage = nullptr;
    std::shared_ptr<gnomonBinaryImageSeries> binaryImage = nullptr;
    std::shared_ptr<gnomonMeshSeries> output = nullptr;
};

gnomonMeshFromImageCommand::gnomonMeshFromImageCommand() : d(new gnomonMeshFromImageCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonMeshFromImageCommand)

    QStringList keys = gnomonCore::meshFromImage::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::meshFromImage::pluginFactory().create(this->algorithm_name);
    }
}

gnomonMeshFromImageCommand::~gnomonMeshFromImageCommand()
{
    delete d;
}

void gnomonMeshFromImageCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

    delete this->action;
    this->action = gnomonCore::meshFromImage::pluginFactory().create(algo_name);
}

void gnomonMeshFromImageCommand::predo(void)
{
    this->action->is_async = true;
}

void gnomonMeshFromImageCommand::postdo(void)
{
    std::shared_ptr<gnomonMeshSeries> mesh = ((gnomonAbstractMeshFromImage *) this->action)->output();

    if ((!mesh)||(mesh->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = mesh;
    }
}

void gnomonMeshFromImageCommand::undo()
{
    ((gnomonAbstractMeshFromImage *) this->action)->setImage(nullptr);
    ((gnomonAbstractMeshFromImage *) this->action)->setCellImage(nullptr);
    ((gnomonAbstractMeshFromImage *) this->action)->setBinaryImage(nullptr);
    this->action->refreshParameters();
}

void gnomonMeshFromImageCommand::setImage(std::shared_ptr<gnomonImageSeries> image)
{
    if ((!image)||(image->times().empty())) {
        d->image = nullptr;
    } else {
        d->image = image;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractMeshFromImage *) this->action)->setImage(d->image);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonImageSeries> gnomonMeshFromImageCommand::image()
{
    return d->image;
}

void gnomonMeshFromImageCommand::setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage)
{
    if ((!cellImage)||(cellImage->times().empty())) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractMeshFromImage *) this->action)->setCellImage(d->cellImage);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonCellImageSeries> gnomonMeshFromImageCommand::cellImage()
{
    return d->cellImage;
}

void gnomonMeshFromImageCommand::setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> binaryImage)
{
    if ((!binaryImage)||(binaryImage->times().empty())) {
        d->binaryImage = nullptr;
    } else {
        d->binaryImage = binaryImage;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractMeshFromImage *) this->action)->setBinaryImage(d->binaryImage);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonBinaryImageSeries> gnomonMeshFromImageCommand::binaryImage()
{
    return d->binaryImage;
}

std::shared_ptr<gnomonMeshSeries> gnomonMeshFromImageCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonMeshFromImageCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["image"] = this->image();
    inputs["cellImage"] = this->cellImage();
    inputs["binaryImage"] = this->binaryImage();
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonMeshFromImageCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonMeshFromImageCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("image", "gnomonImage"));
    input_types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    input_types.emplace_back(std::make_pair("binaryImage", "gnomonBinaryImage"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonMeshFromImageCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonMesh"));
    return output_types;
}

void gnomonMeshFromImageCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "image") {
        this->setImage(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else if (name == "cellImage") {
        this->setCellImage(std::dynamic_pointer_cast<gnomonCellImageSeries>(form));
    } else if (name == "binaryImage") {
        this->setBinaryImage(std::dynamic_pointer_cast<gnomonBinaryImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonMeshFromImageCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonMeshSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonMeshFromImageCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

//
// gnomonMeshFromImageCommand.cpp ends here
