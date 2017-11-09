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

#include <gnomonVisualization>

#include <dtkWidgets>
#include <dtkLog>

#include <dtkImaging>
#include <gnomonCellGraph>

// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeViewPrivate
// /////////////////////////////////////////////////////////////////

class gnomonComposerNodeViewPrivate
{
public:
    static gnomonView *view;

public:
    dtkComposerTransmitterReceiver<dtkImage *> image_receiver;
    dtkComposerTransmitterReceiver<gnomonCellGraph *> cellgraph_receiver;
};

gnomonView *gnomonComposerNodeViewPrivate::view = Q_NULLPTR;

// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeView
// /////////////////////////////////////////////////////////////////

gnomonComposerNodeView::gnomonComposerNodeView(void) : dtkComposerNodeLeaf()
{
    d = new gnomonComposerNodeViewPrivate;

    this->appendReceiver(&(d->image_receiver));
    this->appendReceiver(&(d->cellgraph_receiver));
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

        dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");

        if(!converter)
            return;

        dtkImage *image = d->image_receiver.data();

        converter->setInput(image);

        if(!converter->convert())
            return;

        vtkImageData *data = static_cast<vtkImageData *>(converter->output());

        d->view->manager()->insert(data);
    }

    if (!d->cellgraph_receiver.isEmpty()) {

        gnomonCellGraph *data = d->cellgraph_receiver.data();
        
        d->view->manager()->insert(data);
    }
}

//
// gnomonComposerNodeView.cpp ends here
