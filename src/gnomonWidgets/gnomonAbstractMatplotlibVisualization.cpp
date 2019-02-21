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

#include "gnomonAbstractMatplotlibVisualization.h"
#include "gnomonAbstractMatplotlibVisualization_p.h"

#include <QtWidgets>

#include "gnomonViewMatplotlib.h"


// /////////////////////////////////////////////////////////////////
// gnomonAbstractMatplotlibVisualizationPrivate
// /////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////
// gnomonAbstractMatplotlibVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractMatplotlibVisualization::gnomonAbstractMatplotlibVisualization(void) : d(new gnomonAbstractMatplotlibVisualizationPrivate)
{
}

gnomonAbstractMatplotlibVisualization::~gnomonAbstractMatplotlibVisualization(void)
{
    delete d;

    d = NULL;
}

void gnomonAbstractMatplotlibVisualization::setView(gnomonViewMatplotlib* view)
{
    d->view = view;
}

gnomonViewMatplotlib* gnomonAbstractMatplotlibVisualization::view(void)
{
    return d->view;
}


//
// gnomonAbstractMatplotlibVisualization.cpp ends here
