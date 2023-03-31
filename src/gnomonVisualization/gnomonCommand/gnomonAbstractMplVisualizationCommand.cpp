#include "gnomonAbstractMplVisualizationCommand.h"

#include "gnomonVisualization/gnomonView/gnomonMplView.h"
#include "gnomonVisualization/gnomonVisualizations/gnomonAbstractMplVisualization.h"

void gnomonAbstractMplVisualizationCommand::setView(gnomonAbstractView *view)
{
    if (auto mpl_view = dynamic_cast<gnomonMplView *>(view)) {
        gnomonAbstractVisualizationCommand::setView(mpl_view);
    }
}