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

gnomonCellImageQuantificationCommand::gnomonCellImageQuantificationCommand(const QString& key) : d(new gnomonCellImageQuantificationCommandPrivate)
{
    this->factory_name = "cellImageQuantification";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::cellImageQuantification::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellImageQuantificationCommand::~gnomonCellImageQuantificationCommand(void)
{
    delete d;
}

void gnomonCellImageQuantificationCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageQuantification *) this->action)->cellImage();
    if ((!cellImage)||(cellImage->times().size())==0) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }

    gnomonDataFrameSeries *dataFrame = ((gnomonAbstractCellImageQuantification *) this->action)->dataFrame();
    if ((!dataFrame)||(dataFrame->times().size()==0)) {
        d->dataFrame = nullptr;
    }
    else {
        d->dataFrame = dataFrame;
    }
}

void gnomonCellImageQuantificationCommand::undo(void)
{
    ((gnomonAbstractCellImageQuantification *) this->action)->setImage(nullptr);
    ((gnomonAbstractCellImageQuantification *) this->action)->setCellImage(nullptr);
}

void gnomonCellImageQuantificationCommand::setImage(gnomonImageSeries *image)
{
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        d->image = nullptr;
    } else {
        d->image = image;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageQuantification *) this->action)->setImage(d->image);
    }
}

void gnomonCellImageQuantificationCommand::setCellImage(gnomonCellImageSeries *cellImage)
{
    if ((!cellImage)||(cellImage->times().size()==0)) {
        d->input_cellImage = nullptr;
    } else {
        d->input_cellImage = cellImage;

        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageQuantification *) this->action)->setCellImage(d->input_cellImage);
    }
}

void gnomonCellImageQuantificationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

dtkCoreParameters gnomonCellImageQuantificationCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImageSeries *gnomonCellImageQuantificationCommand::cellImage(void)
{
    return d->cellImage;
}

gnomonDataFrameSeries *gnomonCellImageQuantificationCommand::dataFrame(void)
{
    return d->dataFrame;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageQuantificationCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["image"] = d->image;
    inputs["cellImage"] = d->input_cellImage;
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageQuantificationCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["cellImage"] = this->cellImage();
    outputs["dataFrame"] = this->dataFrame();
    return outputs;
}

bool gnomonCellImageQuantificationCommand::isEmpty(void)
{
    loadPluginGroup("cellImageQuantification");
    return gnomonCore::cellImageQuantification::pluginFactory().keys().size() == 0;
}

//
// gnomonCellImageQuantificationCommand.cpp ends here
