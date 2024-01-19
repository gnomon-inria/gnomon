#include "gnomonCellImageTrackingCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageTracking.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonCellImageTrackingCommandPrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image = nullptr;
    std::shared_ptr<gnomonCellImageSeries> input_cellImage = nullptr;
    std::shared_ptr<gnomonDataDictSeries> transformation = nullptr;

    std::shared_ptr<gnomonCellImageSeries> cellImage = nullptr;
    std::shared_ptr<gnomonTreeSeries> tree = nullptr;
};

gnomonCellImageTrackingCommand::gnomonCellImageTrackingCommand() : d(new gnomonCellImageTrackingCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonCellImageTrackingCommand)

    QStringList keys = gnomonCore::cellImageTracking::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellImageTracking::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellImageTrackingCommand::~gnomonCellImageTrackingCommand()
{
    delete d;
}

void gnomonCellImageTrackingCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellImageTracking::pluginFactory().create(algo_name);
}

void gnomonCellImageTrackingCommand::predo(void)
{
    this->action->is_async = true;
}

void gnomonCellImageTrackingCommand::postdo(void)
{
    std::shared_ptr<gnomonCellImageSeries> cellImage = ((gnomonAbstractCellImageTracking *) this->action)->cellImage();

    if ((!cellImage)||cellImage->times().empty()) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }

    std::shared_ptr<gnomonTreeSeries> tree = ((gnomonAbstractCellImageTracking *) this->action)->tree();

    if ((!tree)||(tree->times().empty())) {
        d->tree = nullptr;
    } else {
        d->tree = tree;
    }
}

void gnomonCellImageTrackingCommand::undo()
{
    ((gnomonAbstractCellImageTracking *) this->action)->setImage(nullptr);
    ((gnomonAbstractCellImageTracking *) this->action)->setCellImage(nullptr);
    this->action->refreshParameters();
}

void gnomonCellImageTrackingCommand::setImage(std::shared_ptr<gnomonImageSeries> image)
{
    if ((!image)||(image->times().empty())|| (image->current()->channels().empty())) {
        d->image = nullptr;
    } else {
        d->image = image;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageTracking *) this->action)->setImage(d->image);
    this->action->refreshParameters();
}

void gnomonCellImageTrackingCommand::setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage)
{
    if ((!cellImage)||(cellImage->times().empty())) {
        d->input_cellImage = nullptr;
    } else {
        d->input_cellImage = cellImage;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageTracking *) this->action)->setCellImage(d->input_cellImage);
    this->action->refreshParameters();
}

void gnomonCellImageTrackingCommand::setTransformation(std::shared_ptr<gnomonDataDictSeries> datadict)
{
    if ((!datadict)||(datadict->times().empty())) {
        d->transformation = nullptr;
    } else {
        d->transformation = datadict;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageTracking *) this->action)->setTransformation(d->transformation);
        this->action->refreshParameters();
    }
}

std::shared_ptr<gnomonCellImageSeries> gnomonCellImageTrackingCommand::cellImage()
{
    return d->cellImage;
}

std::shared_ptr<gnomonTreeSeries> gnomonCellImageTrackingCommand::tree()
{
    return d->tree;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageTrackingCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["image"] = d->image;
    inputs["cellImage"] = d->input_cellImage;
    inputs["transformation"] = d->transformation;
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageTrackingCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["cellImage"] = this->cellImage();
    outputs["tree"] = this->tree();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonCellImageTrackingCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("image", "gnomonImage"));
    input_types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    input_types.emplace_back(std::make_pair("transformation", "gnomonDataDict"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonCellImageTrackingCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    types.emplace_back(std::make_pair("tree", "gnomonTree"));
    return types;
}

void gnomonCellImageTrackingCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "image") {
        this->setImage(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else if (name == "cellImage") {
        this->setCellImage(std::dynamic_pointer_cast<gnomonCellImageSeries>(form));
    } else if (name == "transformation") {
        this->setTransformation(std::dynamic_pointer_cast<gnomonDataDictSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonCellImageTrackingCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->cellImage) {
        d->cellImage = std::make_shared<gnomonCellImageSeries>();
    }
    auto tmp = serialization["cellImage"].toObject();
    d->cellImage->deserialize(tmp);
    if(!d->tree) {
        d->tree = std::make_shared<gnomonTreeSeries>();
    }
    auto tmp2 = serialization["tree"].toObject();
    d->tree->deserialize(tmp2);
}

QJsonObject gnomonCellImageTrackingCommand::serializeResults(void) {
    QJsonObject out;
    out["cellImage"] = d->cellImage->serialize();
    out["tree"] = d->tree->serialize();
    return out;
}

//
// gnomonCellImageTrackingCommand.cpp ends here
