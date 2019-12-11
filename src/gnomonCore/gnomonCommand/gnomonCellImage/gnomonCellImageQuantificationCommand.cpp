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
    gnomonCellImageSeries* cellImage = nullptr;
};

gnomonCellImageQuantificationCommand::gnomonCellImageQuantificationCommand(const QString& key) : d(new gnomonCellImageQuantificationCommandPrivate)
{
    loadPluginGroup("cellImageQuantification");

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
}

void gnomonCellImageQuantificationCommand::undo(void)
{
    ((gnomonAbstractCellImageQuantification *) this->action)->setImage(nullptr);
    ((gnomonAbstractCellImageQuantification *) this->action)->setCellImage(nullptr);
}

void gnomonCellImageQuantificationCommand::setImage(gnomonImageSeries *image)
{
    d->image = image;

    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageQuantification *) this->action)->setImage(d->image);
}


void gnomonCellImageQuantificationCommand::setCellImage(gnomonCellImageSeries *cellImage)
{
    d->cellImage = cellImage;

    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageQuantification *) this->action)->setCellImage(d->cellImage);
}

void gnomonCellImageQuantificationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonCellImageQuantificationCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImageSeries *gnomonCellImageQuantificationCommand::cellImage(void)
{
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageQuantification *) this->action)->cellImage();
    if ((!cellImage)||(cellImage->times().size())==0) {
        return nullptr;
    } else {
        return ((gnomonAbstractCellImageQuantification *) this->action)->cellImage();
    }
}

gnomonDataFrameSeries *gnomonCellImageQuantificationCommand::dataFrame(void)
{
    gnomonDataFrameSeries *dataFrame = ((gnomonAbstractCellImageQuantification *) this->action)->dataFrame();
    if ((!dataFrame)||(dataFrame->times().size()==0)) {
        return nullptr;
    }
    else {
        return ((gnomonAbstractCellImageQuantification *) this->action)->dataFrame();
    }
}

//
// gnomonCellImageQuantificationCommand.cpp ends here
