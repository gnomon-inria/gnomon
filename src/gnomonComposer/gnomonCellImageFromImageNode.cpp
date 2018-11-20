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

#include "gnomonCellImageFromImageNode.h"

#include "gnomonAbstractCellImageFromImage.h"
#include "gnomonCellImage.h"
#include <QtCore>

#include <dtkLog>
#include <dtkImage.h>

class gnomonCellImageFromImageNodePrivate
{
public:
    dtkComposerTransmitterReceiver<dtkImage *> image_in;
    dtkComposerTransmitterReceiver<bool> use_margins;
    dtkComposerTransmitterReceiver<long> background_label;
    dtkComposerTransmitterReceiver<QStringList> property_names;

    dtkComposerTransmitterEmitter<gnomonCellImage *> cellimage_out;
};


gnomonCellImageFromImageNode::gnomonCellImageFromImageNode(void) : dtkComposerNodeObject<gnomonAbstractCellImageFromImage>(), d(new gnomonCellImageFromImageNodePrivate())
{
    this->setFactory(gnomonCore::cellImageFromImage::pluginFactory());

    this->appendReceiver(&d->image_in);
    this->appendReceiver(&d->use_margins);
    this->appendReceiver(&d->background_label);
    this->appendReceiver(&d->property_names);

    this->appendEmitter (&d->cellimage_out);
}

gnomonCellImageFromImageNode::~gnomonCellImageFromImageNode(void)
{
    delete d;
}


void gnomonCellImageFromImageNode::run(void)
{
    if (d->image_in.isEmpty()) {
        dtkError() << Q_FUNC_INFO << "The input image is not set. Aborting.";
        return;

    } else {

        gnomonAbstractCellImageFromImage *cellimageFromImage = this->object();
        if (!cellimageFromImage) {
            dtkError() << Q_FUNC_INFO << "No Graph from Image found. Aborting.";
            return;
        }

        cellimageFromImage->setImage(d->image_in.data());

        if(!d->use_margins.isEmpty()) {
            cellimageFromImage->setUseMargins(d->use_margins.data());
        }
        if(!d->background_label.isEmpty()) {
            cellimageFromImage->setBackgroundLabel(d->background_label.data());
        }
        if(!d->property_names.isEmpty()) {
            cellimageFromImage->setPredefinedProperties(d->property_names.data());
        }

        cellimageFromImage->run();

        gnomonCellImage *cellimage = cellimageFromImage->computedImage();


        d->cellimage_out.setData(cellimage);
    }
}

//
// gnomonCellImageFromImageNode.cpp ends here
