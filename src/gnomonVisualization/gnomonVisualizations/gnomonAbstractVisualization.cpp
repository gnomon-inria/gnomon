#include "gnomonAbstractVisualization.h"
#include "gnomonAbstractVisualization_p.h"

#include <gnomonVisualization/gnomonView/gnomonAbstractView.h>

// /////////////////////////////////////////////////////////////////
// gnomonAbstractVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractVisualization::gnomonAbstractVisualization(void) : d(new gnomonAbstractVisualizationPrivate)
{
}

gnomonAbstractVisualization::~gnomonAbstractVisualization(void)
{
    delete d;
    d = nullptr;
}

void gnomonAbstractVisualization::setView(gnomonAbstractView* view)
{
    d->view = view;
}

gnomonAbstractView* gnomonAbstractVisualization::view(void)
{
    return d->view;
}

//
// gnomonAbstractVisualization.cpp ends here
