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

#include "gnomonComposerNodeView.h"

#include <gnomonView>
#include <gnomonViewManager>

#include <dtkWidgets>
#include <dtkLog>

#include <dtkImage>
#include <gnomonCellComplex>
#include <gnomonCellGraph>
#include <gnomonForm/gnomonCellImage>

// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeViewPrivate
// /////////////////////////////////////////////////////////////////

class gnomonComposerNodeViewPrivate
{
public:
    static gnomonView *view;

public:
    dtkComposerTransmitterReceiver<dtkImage *> image_receiver;
    dtkComposerTransmitterReceiver<gnomonCellComplex *> cellcomplex_receiver;
    dtkComposerTransmitterReceiver<gnomonCellGraph *> cellgraph_receiver;
    dtkComposerTransmitterReceiver<gnomonCellImage *> cellimage_receiver;
};

gnomonView *gnomonComposerNodeViewPrivate::view = Q_NULLPTR;

// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeView
// /////////////////////////////////////////////////////////////////

gnomonComposerNodeView::gnomonComposerNodeView(void) : dtkComposerNodeLeaf()
{
    d = new gnomonComposerNodeViewPrivate;

    this->appendReceiver(&(d->image_receiver));
    this->appendReceiver(&(d->cellcomplex_receiver));
    this->appendReceiver(&(d->cellgraph_receiver));
    this->appendReceiver(&(d->cellimage_receiver));
}

gnomonComposerNodeView::~gnomonComposerNodeView(void)
{
    delete d->view;
    delete d;
}

void gnomonComposerNodeView::run(void)
{
    if(!d->view) {
        d->view = new gnomonView;

        dtkViewController::instance()->insert(d->view);
    }

    if(!d->image_receiver.isEmpty()) {

        dtkImage *data = d->image_receiver.data();

        d->view->manager()->insert(data);
    }

    if (!d->cellcomplex_receiver.isEmpty()) {

        gnomonCellComplex *data = d->cellcomplex_receiver.data();

        d->view->manager()->insert(data);
    }

    if (!d->cellgraph_receiver.isEmpty()) {

        gnomonCellGraph *data = d->cellgraph_receiver.data();

        d->view->manager()->insert(data);
    }

    if (!d->cellimage_receiver.isEmpty()) {

        gnomonCellImage *data = d->cellimage_receiver.data();

        d->view->manager()->insert(data);
    }
}

//
// gnomonComposerNodeView.cpp ends here
