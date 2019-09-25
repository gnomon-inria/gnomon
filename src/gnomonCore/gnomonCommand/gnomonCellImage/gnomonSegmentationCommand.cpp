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
    gnomonImageSeries *images_serie = nullptr;
};

gnomonSegmentationCommand::gnomonSegmentationCommand(const QString& key) : d(new gnomonSegmentationCommandPrivate)
{
    loadPluginGroup("gnomonCellImageFromImage");

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

void gnomonSegmentationCommand::setInput(gnomonImageSeries* images_serie)
{
    d->images_serie = images_serie;
    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(d->images_serie);
}

gnomonImageSeries *gnomonSegmentationCommand::input()
{
    return ((gnomonAbstractCellImageFromImage *) this->action)->input();
}

gnomonCellImageSeries *gnomonSegmentationCommand::output()
{
    return ((gnomonAbstractCellImageFromImage *) this->action)->output();
}

QMap<QString, gnomonCoreParameter *> gnomonSegmentationCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonSegmentationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

//
// gnomonSegmentationCommand.cpp ends here
