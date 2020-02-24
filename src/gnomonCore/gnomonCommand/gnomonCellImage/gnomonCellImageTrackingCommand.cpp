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
    gnomonCellImageSeries* cellImage = nullptr;
};

gnomonCellImageTrackingCommand::gnomonCellImageTrackingCommand(const QString& key) : d(new gnomonCellImageTrackingCommandPrivate)
{
    loadPluginGroup("cellImageTracking");

    this->action = gnomonCore::cellImageTracking::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellImageTrackingCommand::~gnomonCellImageTrackingCommand(void)
{
    delete d;
}

void gnomonCellImageTrackingCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonCellImageTrackingCommand::undo(void)
{
    ((gnomonAbstractCellImageTracking *) this->action)->setImage(nullptr);
    ((gnomonAbstractCellImageTracking *) this->action)->setCellImage(nullptr);
}

void gnomonCellImageTrackingCommand::setImage(gnomonImageSeries *image)
{
    d->image = image;

    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageTracking *) this->action)->setImage(d->image);
}


void gnomonCellImageTrackingCommand::setCellImage(gnomonCellImageSeries *cellImage)
{
    d->cellImage = cellImage;

    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageTracking *) this->action)->setCellImage(d->cellImage);
}

void gnomonCellImageTrackingCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonCellImageTrackingCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImageSeries *gnomonCellImageTrackingCommand::cellImage(void)
{
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageTracking *) this->action)->cellImage();
    if ((!cellImage)||(cellImage->times().size())==0) {
        return nullptr;
    } else {
        return cellImage;
    }
}

gnomonTreeSeries *gnomonCellImageTrackingCommand::tree(void)
{
    gnomonTreeSeries *tree = ((gnomonAbstractCellImageTracking *) this->action)->tree();
    if ((!tree)||(tree->times().size()==0)) {
        return nullptr;
    }
    else {
        return tree;
    }
}

//
// gnomonCellImageTrackingCommand.cpp ends here
