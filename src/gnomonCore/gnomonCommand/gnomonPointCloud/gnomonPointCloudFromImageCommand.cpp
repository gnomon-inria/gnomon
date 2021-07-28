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

#include "gnomonPointCloudFromImageCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonPointCloudFromImageCommandPrivate
{
public:
    gnomonImageSeries* input = nullptr;

    gnomonPointCloudSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonPointCloudFromImageCommand::gnomonPointCloudFromImageCommand(const QString& key) : d(new gnomonPointCloudFromImageCommandPrivate)
{
    this->factory_name = "pointCloudFromImage";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::pointCloudFromImage::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonPointCloudFromImageCommand::~gnomonPointCloudFromImageCommand(void)
{
    delete d;
}

void gnomonPointCloudFromImageCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudFromImage *) this->action)->output();
    if ((!pointCloud)||(pointCloud->times().size()==0)) {
        d->output = nullptr;
    } else {
        d-> output = pointCloud;
    }
}

void gnomonPointCloudFromImageCommand::undo(void)
{
    ((gnomonAbstractPointCloudFromImage *) this->action)->setInput(nullptr);
}

void gnomonPointCloudFromImageCommand::setInput(gnomonImageSeries *image)
{
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = image;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractPointCloudFromImage *) this->action)->setInput(d->input);
}

void gnomonPointCloudFromImageCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, dtkCoreParameter *> gnomonPointCloudFromImageCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImageSeries *gnomonPointCloudFromImageCommand::input(void)
{
    return d->input;
}

gnomonPointCloudSeries *gnomonPointCloudFromImageCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudFromImageCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudFromImageCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonPointCloudFromImageCommand::isEmpty(void)
{
    loadPluginGroup("pointCloudFromImage");
    return gnomonCore::pointCloudFromImage::pluginFactory().keys().size() == 0;
}

//
// gnomonPointCloudFromImageCommand.cpp ends here
