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

#include "gnomonCellImageTrackingCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonCellImageTrackingCommandPrivate
{
public:
    gnomonImageSeries* image = nullptr;
    gnomonCellImageSeries* input_cellImage = nullptr;

    gnomonCellImageSeries* cellImage = nullptr;
    gnomonTreeSeries* tree = nullptr;
};

gnomonCellImageTrackingCommand::gnomonCellImageTrackingCommand(void) : d(new gnomonCellImageTrackingCommandPrivate)
{
    this->factory_name = "cellImageTracking";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellImageTracking::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellImageTracking::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellImageTrackingCommand::~gnomonCellImageTrackingCommand(void)
{
    delete d;
}

void gnomonCellImageTrackingCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::cellImageTracking::pluginFactory().create(algo_name);
}

void gnomonCellImageTrackingCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageTracking *) this->action)->cellImage();
    if ((!cellImage)||(cellImage->times().size())==0) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }

    gnomonTreeSeries *tree = ((gnomonAbstractCellImageTracking *) this->action)->tree();
    if ((!tree)||(tree->times().size()==0)) {
        d->tree = nullptr;
    }
    else {
        d->tree = tree;
    }
}

void gnomonCellImageTrackingCommand::undo(void)
{
    ((gnomonAbstractCellImageTracking *) this->action)->setImage(nullptr);
    ((gnomonAbstractCellImageTracking *) this->action)->setCellImage(nullptr);
}

void gnomonCellImageTrackingCommand::setImage(gnomonImageSeries *image)
{
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        d->image = nullptr;
    } else {
        d->image = image;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageTracking *) this->action)->setImage(d->image);
    }
}

void gnomonCellImageTrackingCommand::setCellImage(gnomonCellImageSeries *cellImage)
{
    if ((!cellImage)||(cellImage->times().size()==0)) {
        d->input_cellImage = nullptr;
    } else {
        d->input_cellImage = cellImage;

        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageTracking *) this->action)->setCellImage(d->input_cellImage);
    }
}

void gnomonCellImageTrackingCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

dtkCoreParameters gnomonCellImageTrackingCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImageSeries *gnomonCellImageTrackingCommand::cellImage(void)
{
    return d->cellImage;
}

gnomonTreeSeries *gnomonCellImageTrackingCommand::tree(void)
{
    return d->tree;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageTrackingCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["image"] = d->image;
    inputs["cellImage"] = d->input_cellImage;
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageTrackingCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["cellImage"] = this->cellImage();
    outputs["tree"] = this->tree();
    return outputs;
}

bool gnomonCellImageTrackingCommand::isEmpty(void)
{
    loadPluginGroup("cellImageTracking");
    return gnomonCore::cellImageTracking::pluginFactory().keys().size() == 0;
}

//
// gnomonCellImageTrackingCommand.cpp ends here
