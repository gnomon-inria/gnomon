#include "gnomonAbstractMatplotlibVisualization.h"
#include "gnomonAbstractVisualization_p.h"

#include "gnomonView/gnomonViewMatplotlib.h"


// /////////////////////////////////////////////////////////////////
// gnomonAbstractMatplotlibVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractMatplotlibVisualization::gnomonAbstractMatplotlibVisualization(void) : gnomonAbstractVisualization()
{
}

gnomonAbstractMatplotlibVisualization::~gnomonAbstractMatplotlibVisualization(void)
{
}

void gnomonAbstractMatplotlibVisualization::setView(gnomonAbstractView* view)
{
    if (auto mpl_view = dynamic_cast<gnomonViewMatplotlib *>(view)) {
        gnomonAbstractVisualization::setView(mpl_view);
    }
}

int gnomonAbstractMatplotlibVisualization::figureNumber(void) {
    if (auto mpl_view = dynamic_cast<gnomonViewMatplotlib *>(d->view)) {
        return mpl_view->figureNumber();
    } else {
        return -1;
    }
}

//
// gnomonAbstractMatplotlibVisualization.cpp ends here
