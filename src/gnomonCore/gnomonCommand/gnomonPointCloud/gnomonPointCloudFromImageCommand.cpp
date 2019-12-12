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
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonPointCloudFromImageCommand::gnomonPointCloudFromImageCommand(const QString& key) : d(new gnomonPointCloudFromImageCommandPrivate)
{
    loadPluginGroup("pointCloudFromImage");

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
}

void gnomonPointCloudFromImageCommand::undo(void)
{
    ((gnomonAbstractPointCloudFromImage *) this->action)->setInput(nullptr);
}

void gnomonPointCloudFromImageCommand::setInput(gnomonImageSeries *input)
{
    d->input = input;

    Q_ASSERT(this->action);
    ((gnomonAbstractPointCloudFromImage *) this->action)->setInput(d->input);
}

void gnomonPointCloudFromImageCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonPointCloudFromImageCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImageSeries *gnomonPointCloudFromImageCommand::input(void)
{
    gnomonImageSeries *image = ((gnomonAbstractPointCloudFromImage *) this->action)->input();
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        return nullptr;
    } else {
        return image;
    }
}

gnomonPointCloudSeries *gnomonPointCloudFromImageCommand::output(void)
{
    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudFromImage *) this->action)->output();
    if ((!pointCloud)||(pointCloud->times().size()==0)) {
        return nullptr;
    } else {
        return pointCloud;
    }
}

//
// gnomonPointCloudFromImageCommand.cpp ends here
