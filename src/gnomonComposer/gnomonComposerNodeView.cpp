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

// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeViewPrivate
// /////////////////////////////////////////////////////////////////

class gnomonComposerNodeViewPrivate
{
public:
    static gnomonView *view;

public:
    dtkComposerTransmitterReceiver<dtkImage *> receiver;
};

gnomonView *gnomonComposerNodeViewPrivate::view = Q_NULLPTR;

// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeView
// /////////////////////////////////////////////////////////////////

gnomonComposerNodeView::gnomonComposerNodeView(void) : dtkComposerNodeLeaf()
{
    d = new gnomonComposerNodeViewPrivate;

    this->appendReceiver(&(d->receiver));
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

    if(!d->receiver.isEmpty()) {

        dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");

        if(!converter)
            return;

        dtkImage *image = d->receiver.data();

        qDebug() << Q_FUNC_INFO << image->dim() << image->storageType() << image->xDim() << image->yDim() << image->zDim();

        converter->setInput(image);

        if(!converter->convert())
            return;

        vtkImageData *data = static_cast<vtkImageData *>(converter->output());

        data->PrintSelf(std::cout, vtkIndent());

        d->view->manager()->insert(data);
    }
}

//
// gnomonComposerNodeView.cpp ends here
