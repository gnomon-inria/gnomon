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

#include "tissueComposerNodeView.h"

#include <tissueVisualization>

#include <dtkWidgets>
#include <dtkLog>

// /////////////////////////////////////////////////////////////////
// tissueComposerNodeViewPrivate
// /////////////////////////////////////////////////////////////////

class tissueComposerNodeViewPrivate
{
public:
    tissueView *view;
};

// /////////////////////////////////////////////////////////////////
// tissueComposerNodeView
// /////////////////////////////////////////////////////////////////

tissueComposerNodeView::tissueComposerNodeView(void) : dtkComposerNodeLeaf()
{
    d = new tissueComposerNodeViewPrivate;
    d->view = Q_NULLPTR;
}

tissueComposerNodeView::~tissueComposerNodeView(void)
{
    delete d->view;
    delete d;
}

void tissueComposerNodeView::run(void)
{
    if(d->view)
        return;

    d->view = new tissueView;

    dtkViewController::instance()->insert(d->view);
}

//
// tissueComposerNodeView.cpp ends here
