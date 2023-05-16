#include "gnomonAbstractQmlVisualizationCommand.h"

#include "gnomonVisualization/gnomonView/gnomonQmlView.h"
#include "gnomonVisualization/gnomonVisualizations/gnomonAbstractQmlVisualization.h"

void gnomonAbstractQmlVisualizationCommand::setView(gnomonAbstractView *view)
{
    if (auto qml_view = dynamic_cast<gnomonQmlView *>(view)) {
        gnomonAbstractVisualizationCommand::setView(qml_view);
    }
}