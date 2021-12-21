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

#include "gnomonCellImageQuantificationCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonCellImageQuantificationCommandPrivate
{
public:
    gnomonImageSeries* image = nullptr;
    gnomonCellImageSeries* input_cellImage = nullptr;

    gnomonCellImageSeries* cellImage = nullptr;
    gnomonDataFrameSeries* dataFrame = nullptr;
};

gnomonCellImageQuantificationCommand::gnomonCellImageQuantificationCommand() : d(new gnomonCellImageQuantificationCommandPrivate)
{
    this->factory_name = "cellImageQuantification";
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

void gnomonCellImageQuantificationCommand::redo()
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageQuantification *) this->action)->cellImage();
    if ((!cellImage)||cellImage->times().empty()) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }

    gnomonDataFrameSeries *dataFrame = ((gnomonAbstractCellImageQuantification *) this->action)->dataFrame();
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
}

void gnomonCellImageQuantificationCommand::setImage(gnomonImageSeries *image)
{
    if ((!image)||(image->times().empty())||(((gnomonImage *)image->current())->channels().empty())) {
        d->image = nullptr;
    } else {
        d->image = image;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageQuantification *) this->action)->setImage(d->image);
    }
}

void gnomonCellImageQuantificationCommand::setCellImage(gnomonCellImageSeries *cellImage)
{
    if ((!cellImage)||(cellImage->times().empty())) {
        d->input_cellImage = nullptr;
    } else {
        d->input_cellImage = cellImage;

        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageQuantification *) this->action)->setCellImage(d->input_cellImage);
    }
}

gnomonCellImageSeries *gnomonCellImageQuantificationCommand::cellImage()
{
    return d->cellImage;
}

gnomonDataFrameSeries *gnomonCellImageQuantificationCommand::dataFrame()
{
    return d->dataFrame;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageQuantificationCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["image"] = d->image;
    inputs["cellImage"] = d->input_cellImage;
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageQuantificationCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["cellImage"] = this->cellImage();
    outputs["dataFrame"] = this->dataFrame();
    return outputs;
}

bool gnomonCellImageQuantificationCommand::isEmpty()
{
    loadPluginGroup("cellImageQuantification");
    return gnomonCore::cellImageQuantification::pluginFactory().keys().empty();
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

//
// gnomonCellImageQuantificationCommand.cpp ends here
