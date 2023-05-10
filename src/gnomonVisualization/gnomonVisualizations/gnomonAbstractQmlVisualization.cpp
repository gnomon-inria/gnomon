#include "gnomonAbstractQmlVisualization.h"
#include "gnomonAbstractVisualization_p.h"

#include "gnomonView/gnomonQmlView.h"


// /////////////////////////////////////////////////////////////////
// gnomonAbstractQmlVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractQmlVisualization::gnomonAbstractQmlVisualization(void) : gnomonAbstractVisualization()
{
}

gnomonAbstractQmlVisualization::~gnomonAbstractQmlVisualization(void)
{
}

gnomonQmlView *gnomonAbstractQmlVisualization::qmlView(void)
{
    if (auto qml_view = dynamic_cast<gnomonQmlView *>(d->view)) {
        return qml_view;
    } else {
        return nullptr;
    }
}

//
// gnomonAbstractQmlVisualization.cpp ends here
