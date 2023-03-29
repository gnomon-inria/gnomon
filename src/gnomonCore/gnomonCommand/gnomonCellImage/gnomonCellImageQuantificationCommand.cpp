#include "gnomonCellImageQuantificationCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageQuantification.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonCellImageQuantificationCommandPrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image = nullptr;
    std::shared_ptr<gnomonCellImageSeries> input_cellImage = nullptr;

    std::shared_ptr<gnomonCellImageSeries> cellImage = nullptr;
    std::shared_ptr<gnomonDataFrameSeries> dataFrame = nullptr;
};

gnomonCellImageQuantificationCommand::gnomonCellImageQuantificationCommand() : d(new gnomonCellImageQuantificationCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

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

}

void gnomonCellImageQuantificationCommand::postdo(void)
{
    std::shared_ptr<gnomonCellImageSeries> cellImage = ((gnomonAbstractCellImageQuantification *) this->action)->cellImage();

    if ((!cellImage)||cellImage->times().empty()) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }

    std::shared_ptr<gnomonDataFrameSeries> dataFrame = ((gnomonAbstractCellImageQuantification *) this->action)->dataFrame();

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
    ((gnomonAbstractCellImageQuantification *) this->action)->setCellImage(nullptr);
    this->action->refreshParameters();
}

void gnomonCellImageQuantificationCommand::setImage(std::shared_ptr<gnomonImageSeries> image)
{
    if ((!image)||(image->times().empty())||(image->current()->channels().empty())) {
        d->image = nullptr;
    } else {
        d->image = image;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageQuantification *) this->action)->setImage(d->image);
        this->action->refreshParameters();
    }
}

void gnomonCellImageQuantificationCommand::setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage)
{
    if ((!cellImage)||(cellImage->times().empty())) {
        d->input_cellImage = nullptr;
    } else {
        d->input_cellImage = cellImage;

        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageQuantification *) this->action)->setCellImage(d->input_cellImage);
        this->action->refreshParameters();
    }
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
    inputs["cellImage"] = d->input_cellImage;
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageQuantificationCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["cellImage"] = this->cellImage();
    outputs["dataFrame"] = this->dataFrame();
    return outputs;
}

bool gnomonCellImageQuantificationCommand::isEmpty()
{
    return availablePlugins().empty();
}

gnomonAbstractCommand::orderedMap gnomonCellImageQuantificationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("image", "gnomonImage"));
    input_types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonCellImageQuantificationCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    types.emplace_back(std::make_pair("dataFrame", "gnomonDataFrame"));
    return types;
}

void gnomonCellImageQuantificationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "image") {
        this->setImage(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else if (name == "cellImage") {
        this->setCellImage(std::dynamic_pointer_cast<gnomonCellImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

QStringList gnomonCellImageQuantificationCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

void gnomonCellImageQuantificationCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->cellImage) {
        d->cellImage = std::make_shared<gnomonCellImageSeries>();
    }
    auto tmp = serialization["cellImage"].toObject();
    d->cellImage->deserialize(tmp);
    if(!d->dataFrame) {
        d->dataFrame = std::make_shared<gnomonDataFrameSeries>();
    }
    auto tmp2 = serialization["dataFrame"].toObject();
    d->dataFrame->deserialize(tmp2);
}

QJsonObject gnomonCellImageQuantificationCommand::serializeResults(void) {
    QJsonObject out;
    out["cellImage"] = d->cellImage->serialize();
    out["dataFrame"] = d->dataFrame->serialize();
    return out;
}

//
// gnomonCellImageQuantificationCommand.cpp ends here
