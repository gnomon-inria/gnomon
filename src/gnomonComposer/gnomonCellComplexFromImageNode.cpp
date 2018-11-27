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

#include "gnomonCellComplexFromImageNode.h"

#include "gnomonAbstractCellComplexFromImage.h"
#include "gnomonCellComplex.h"
#include <QtCore>

#include <dtkLog>
#include <dtkImage.h>

class gnomonCellComplexFromImageNodePrivate
{
public:
    dtkComposerTransmitterReceiver<dtkImage *> image_in;
    dtkComposerTransmitterReceiver<int> dimension;

    dtkComposerTransmitterEmitter<gnomonCellComplex *> cellcomplex_out;
    dtkComposerTransmitterEmitter<gnomonCellComplex *> adjacency_out;
};


gnomonCellComplexFromImageNode::gnomonCellComplexFromImageNode(void) : dtkComposerNodeObject<gnomonAbstractCellComplexFromImage>(), d(new gnomonCellComplexFromImageNodePrivate())
{
    this->setFactory(gnomonCore::cellComplexFromImage::pluginFactory());

    this->appendReceiver(&d->image_in);
    this->appendReceiver(&d->dimension);

    this->appendEmitter (&d->cellcomplex_out);
    this->appendEmitter (&d->adjacency_out);
}

gnomonCellComplexFromImageNode::~gnomonCellComplexFromImageNode(void)
{
    delete d;
}


void gnomonCellComplexFromImageNode::run(void)
{
    if (d->image_in.isEmpty()) {
        dtkError() << Q_FUNC_INFO << "The input image is not set. Aborting.";
        return;

    } else {

        gnomonAbstractCellComplexFromImage *cellcomplexFromImage = this->object();
        if (!cellcomplexFromImage) {
            dtkError() << Q_FUNC_INFO << "No Graph from Image found. Aborting.";
            return;
        }
        cellcomplexFromImage->setImage(d->image_in.data());
        cellcomplexFromImage->setComplexDimension(d->dimension.data());

        cellcomplexFromImage->run();

        d->cellcomplex_out.setData(cellcomplexFromImage->computedComplex());
        d->adjacency_out.setData(cellcomplexFromImage->adjacencyComplex());
    }
}

//
// gnomonCellComplexFromImageNode.cpp ends here
