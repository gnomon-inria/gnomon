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

#include "gnomonCellImageFromImageCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonCellImageFromImageCommandPrivate
{
public:
    gnomonImageSeries *image_series = nullptr;
    gnomonPointCloudSeries *pointCloud_series = nullptr;

    gnomonCellImageSeries *output = nullptr;
};

gnomonCellImageFromImageCommand::gnomonCellImageFromImageCommand(const QString& key) : d(new gnomonCellImageFromImageCommandPrivate)
{
    this->factory_name = "cellImageFromImage";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::cellImageFromImage::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellImageFromImageCommand::~gnomonCellImageFromImageCommand(void)
{
    delete d;
}

void gnomonCellImageFromImageCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageFromImage *) this->action)->output();
    if ((!cellImage)||(cellImage->times().size())==0) {
        d->output = nullptr;
    } else {
        d->output = cellImage;
    }
}

void gnomonCellImageFromImageCommand::undo(void)
{
    Q_ASSERT(this->action);

    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(nullptr);
}

void gnomonCellImageFromImageCommand::setInput(gnomonImageSeries* image_series)
{
    if ((!image_series)||(image_series->times().size()==0)||(((gnomonImage *)image_series->current())->channels().size()==0)) {
        d->image_series = nullptr;
    } else {
        d->image_series = image_series;
    }
    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(d->image_series);
}

gnomonImageSeries *gnomonCellImageFromImageCommand::input()
{
    return d->image_series;
}

void gnomonCellImageFromImageCommand::setCellPoints(gnomonPointCloudSeries *pointCloud_series)
{
    if ((!pointCloud_series)||(pointCloud_series->times().size()==0)) {
        d->pointCloud_series = nullptr;
    } else {
        d->pointCloud_series = pointCloud_series;
    }
    ((gnomonAbstractCellImageFromImage *) this->action)->setCellPoints(d->pointCloud_series);
}

gnomonPointCloudSeries *gnomonCellImageFromImageCommand::cellPoints()
{
    return d->pointCloud_series;
}

gnomonCellImageSeries *gnomonCellImageFromImageCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageFromImageCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    inputs["cellPoints"] = this->cellPoints();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageFromImageCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

QMap<QString, dtkCoreParameter *> gnomonCellImageFromImageCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonCellImageFromImageCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

bool gnomonCellImageFromImageCommand::isEmpty(void)
{
    loadPluginGroup("cellImageFromImage");
    return gnomonCore::cellImageFromImage::pluginFactory().keys().size() == 0;
}

//
// gnomonCellImageFromImageCommand.cpp ends here
