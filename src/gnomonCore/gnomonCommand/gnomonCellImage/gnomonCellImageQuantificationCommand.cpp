#include "gnomonCellImageQuantificationCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageQuantification.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonCellImageQuantificationCommandPrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image = nullptr;
    std::shared_ptr<gnomonMeshSeries> mesh = nullptr;
    std::shared_ptr<gnomonCellImageSeries> input_cellImage = nullptr;

    std::shared_ptr<gnomonCellImageSeries> cellImage = nullptr;
    std::shared_ptr<gnomonDataFrameSeries> dataFrame = nullptr;
};

gnomonCellImageQuantificationCommand::gnomonCellImageQuantificationCommand() : d(new gnomonCellImageQuantificationCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonCellImageQuantificationCommand)

    QStringList keys = gnomonCore::cellImageQuantification::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellImageQuantification::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellImageQuantificationCommand::~gnomonCellImageQuantificationCommand()
{
    delete d;
}

void gnomonCellImageQuantificationCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellImageQuantification::pluginFactory().create(algo_name);
}

void gnomonCellImageQuantificationCommand::predo(void)
{
    this->action->is_async = true;
}

void gnomonCellImageQuantificationCommand::postdo(void)
{
    std::shared_ptr<gnomonCellImageSeries> cellImage = ((gnomonAbstractCellImageQuantification *) this->action)->outputCellImage();

    if ((!cellImage)||cellImage->times().empty()) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }

    std::shared_ptr<gnomonDataFrameSeries> dataFrame = ((gnomonAbstractCellImageQuantification *) this->action)->outputDataFrame();

    if ((!dataFrame)||(dataFrame->times().empty())) {
        d->dataFrame = nullptr;
    }
    else {
        d->dataFrame = dataFrame;
    }
}

void gnomonCellImageQuantificationCommand::undo()
{
    ((gnomonAbstractCellImageQuantification *) this->action)->setImage(nullptr);
    ((gnomonAbstractCellImageQuantification *) this->action)->setMesh(nullptr);
    ((gnomonAbstractCellImageQuantification *) this->action)->setCellImage(nullptr);
    this->action->refreshParameters();
}

void gnomonCellImageQuantificationCommand::setImage(std::shared_ptr<gnomonImageSeries> image)
{
    if ((!image)||(image->times().empty())||(image->current()->channels().empty())) {
        d->image = nullptr;
    } else {
        d->image = image;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageQuantification *) this->action)->setImage(d->image);
    this->action->refreshParameters();
}

void gnomonCellImageQuantificationCommand::setMesh(std::shared_ptr<gnomonMeshSeries> mesh)
{
    if ((!mesh)||(mesh->times().empty())) {
        d->mesh = nullptr;
    } else {
        d->mesh = mesh;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageQuantification *) this->action)->setMesh(d->mesh);
    this->action->refreshParameters();
}

void gnomonCellImageQuantificationCommand::setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage)
{
    if ((!cellImage)||(cellImage->times().empty())) {
        d->input_cellImage = nullptr;
    } else {
        d->input_cellImage = cellImage;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageQuantification *) this->action)->setCellImage(d->input_cellImage);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonCellImageSeries> gnomonCellImageQuantificationCommand::cellImage()
{
    return d->cellImage;
}

std::shared_ptr<gnomonDataFrameSeries> gnomonCellImageQuantificationCommand::dataFrame()
{
    return d->dataFrame;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageQuantificationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["image"] = d->image;
    inputs["mesh"] = d->mesh;
    inputs["cellImage"] = d->input_cellImage;
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageQuantificationCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["outputCellImage"] = this->cellImage();
    outputs["outputDataFrame"] = this->dataFrame();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonCellImageQuantificationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("image", "gnomonImage"));
    input_types.emplace_back(std::make_pair("mesh", "gnomonMesh"));
    input_types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonCellImageQuantificationCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("outputCellImage", "gnomonCellImage"));
    types.emplace_back(std::make_pair("outputDataFrame", "gnomonDataFrame"));
    return types;
}

void gnomonCellImageQuantificationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "cellImage") {
        this->setCellImage(std::dynamic_pointer_cast<gnomonCellImageSeries>(form));
    } else if (name == "image") {
        this->setImage(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else if (name == "mesh") {
        this->setMesh(std::dynamic_pointer_cast<gnomonMeshSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonCellImageQuantificationCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->cellImage) {
        d->cellImage = std::make_shared<gnomonCellImageSeries>();
    }
    auto tmp = serialization["outputCellImage"].toObject();
    d->cellImage->deserialize(tmp);
    if(!d->dataFrame) {
        d->dataFrame = std::make_shared<gnomonDataFrameSeries>();
    }
    auto tmp2 = serialization["outputDataFrame"].toObject();
    d->dataFrame->deserialize(tmp2);
}

QJsonObject gnomonCellImageQuantificationCommand::serializeResults(void) {
    QJsonObject out;
    out["outputCellImage"] = d->cellImage->serialize();
    out["outputDataFrame"] = d->dataFrame->serialize();
    return out;
}

//
// gnomonCellImageQuantificationCommand.cpp ends here
