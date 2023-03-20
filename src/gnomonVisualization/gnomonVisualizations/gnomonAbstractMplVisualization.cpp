#include "gnomonAbstractMplVisualization.h"
#include "gnomonAbstractVisualization_p.h"

#include "gnomonView/gnomonMplView.h"


// /////////////////////////////////////////////////////////////////
// gnomonAbstractMplVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractMplVisualization::gnomonAbstractMplVisualization(void) : gnomonAbstractVisualization()
{
}

gnomonAbstractMplVisualization::~gnomonAbstractMplVisualization(void)
{
}

void gnomonAbstractMplVisualization::setView(gnomonAbstractView* view)
{
    if (auto mpl_view = dynamic_cast<gnomonMplView *>(view)) {
        gnomonAbstractVisualization::setView(mpl_view);
    }
}

int gnomonAbstractMplVisualization::figureNumber(void) {
    if (auto mpl_view = dynamic_cast<gnomonMplView *>(d->view)) {
        return mpl_view->figureNumber();
    } else {
        return -1;
    }
}

//
// gnomonAbstractMplVisualization.cpp ends here
