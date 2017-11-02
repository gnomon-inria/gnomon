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

// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeViewPrivate
// /////////////////////////////////////////////////////////////////

class gnomonComposerNodeViewPrivate
{
public:
    gnomonView *view;
};

// /////////////////////////////////////////////////////////////////
// gnomonComposerNodeView
// /////////////////////////////////////////////////////////////////

gnomonComposerNodeView::gnomonComposerNodeView(void) : dtkComposerNodeLeaf()
{
    d = new gnomonComposerNodeViewPrivate;
    d->view = Q_NULLPTR;
}

gnomonComposerNodeView::~gnomonComposerNodeView(void)
{
    delete d->view;
    delete d;
}

void gnomonComposerNodeView::run(void)
{
    if(d->view)
        return;

    d->view = new gnomonView;

    dtkViewController::instance()->insert(d->view);
}

//
// gnomonComposerNodeView.cpp ends here
