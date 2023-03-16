#include "gnomonAbstractMplVisualizationCommand.h"

#include "gnomonVisualization/gnomonView/gnomonViewMatplotlib.h"
#include "gnomonVisualization/gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"

void gnomonAbstractMplVisualizationCommand::setView(gnomonAbstractView *view)
{
    if (auto mpl_view = dynamic_cast<gnomonViewMatplotlib *>(view)) {
        this->_view = mpl_view;
    }
}