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

#include "gnomonSegmentationCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonSegmentationCommandPrivate
{
public:
    gnomonImageSeries *image_series = nullptr;
    gnomonPointCloudSeries *pointCloud_series = nullptr;
};

gnomonSegmentationCommand::gnomonSegmentationCommand(const QString& key) : d(new gnomonSegmentationCommandPrivate)
{
    loadPluginGroup("cellImageFromImage");

    this->action = gnomonCore::cellImageFromImage::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonSegmentationCommand::~gnomonSegmentationCommand(void)
{
    delete d;
}

void gnomonSegmentationCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonSegmentationCommand::undo(void)
{
    Q_ASSERT(this->action);

    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(nullptr);
}

void gnomonSegmentationCommand::setInput(gnomonImageSeries* image_series)
{
    d->image_series = image_series;
    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(d->image_series);
}

gnomonImageSeries *gnomonSegmentationCommand::input()
{
    gnomonImageSeries *image = ((gnomonAbstractCellImageFromImage *) this->action)->input();
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        return nullptr;
    } else {
        return image;
    }
}

void gnomonSegmentationCommand::setCellPoints(gnomonPointCloudSeries *pointCloud_series)
{
    d->pointCloud_series = pointCloud_series;
    ((gnomonAbstractCellImageFromImage *) this->action)->setCellPoints(d->pointCloud_series);
}

gnomonPointCloudSeries *gnomonSegmentationCommand::cellPoints()
{
    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractCellImageFromImage *) this->action)->cellPoints();
    if ((!pointCloud)||(pointCloud->times().size()==0)) {
        return nullptr;
    } else {
        return pointCloud;
    }
}
    
gnomonCellImageSeries *gnomonSegmentationCommand::output()
{
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageFromImage *) this->action)->output();
    if ((!cellImage)||(cellImage->times().size())==0) {
        return nullptr;
    } else {
        return cellImage;
    }
}

QMap<QString, gnomonCoreParameter *> gnomonSegmentationCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonSegmentationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

bool gnomonSegmentationCommand::isEmpty(void)
{
    loadPluginGroup("cellImageFromImage");
    return gnomonCore::cellImageFromImage::pluginFactory().keys().size() == 0;
}

//
// gnomonSegmentationCommand.cpp ends here
