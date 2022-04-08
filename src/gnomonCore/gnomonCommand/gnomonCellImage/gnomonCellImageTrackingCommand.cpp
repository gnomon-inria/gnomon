#include "gnomonCellImageTrackingCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageTracking.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonCellImageTrackingCommandPrivate
{
public:
    gnomonImageSeries* image = nullptr;
    gnomonCellImageSeries* input_cellImage = nullptr;
    gnomonDataDictSeries* transformation = nullptr;

    gnomonCellImageSeries* cellImage = nullptr;
    gnomonTreeSeries* tree = nullptr;
};

gnomonCellImageTrackingCommand::gnomonCellImageTrackingCommand() : d(new gnomonCellImageTrackingCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

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

}

void gnomonCellImageTrackingCommand::postdo(void)
{
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageTracking *) this->action)->cellImage();

    if ((!cellImage)||cellImage->times().empty()) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }

    gnomonTreeSeries *tree = ((gnomonAbstractCellImageTracking *) this->action)->tree();

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
}

void gnomonCellImageTrackingCommand::setImage(gnomonImageSeries *image)
{
    if ((!image)||(image->times().empty())||(((gnomonImage *)image->current())->channels().empty())) {
        d->image = nullptr;
    } else {
        d->image = image;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageTracking *) this->action)->setImage(d->image);
    }
}

void gnomonCellImageTrackingCommand::setCellImage(gnomonCellImageSeries *cellImage)
{
    if ((!cellImage)||(cellImage->times().empty())) {
        d->input_cellImage = nullptr;
    } else {
        d->input_cellImage = cellImage;

        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageTracking *) this->action)->setCellImage(d->input_cellImage);
    }
}

void gnomonCellImageTrackingCommand::setTransformation(gnomonDataDictSeries *datadict)
{
    if ((!datadict)||(datadict->times().empty())) {
        d->transformation = nullptr;
    } else {
        d->transformation = datadict;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageTracking *) this->action)->setTransformation(d->transformation);
    }
}
gnomonCellImageSeries *gnomonCellImageTrackingCommand::cellImage()
{
    return d->cellImage;
}

gnomonTreeSeries *gnomonCellImageTrackingCommand::tree()
{
    return d->tree;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageTrackingCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["image"] = d->image;
    inputs["cellImage"] = d->input_cellImage;
    inputs["transformation"] = d->transformation;
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageTrackingCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["cellImage"] = this->cellImage();
    outputs["tree"] = this->tree();
    return outputs;
}

bool gnomonCellImageTrackingCommand::isEmpty()
{
    return availablePlugins().empty();
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

void gnomonCellImageTrackingCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "image") {
        this->setImage(dynamic_cast<gnomonImageSeries *>(form));
    } else if (name == "cellImage") {
        this->setCellImage(dynamic_cast<gnomonCellImageSeries *>(form));
    } else if (name == "transformation") {
        this->setTransformation(dynamic_cast<gnomonDataDictSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

QStringList gnomonCellImageTrackingCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

void gnomonCellImageTrackingCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->cellImage) {
        d->cellImage = new gnomonCellImageSeries();
    }
    auto tmp = serialization["cellImage"].toObject();
    d->cellImage->deserialize(tmp);
    if(!d->tree) {
        d->tree = new gnomonTreeSeries();
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
