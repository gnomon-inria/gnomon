#include "gnomonAbstractVtkVisualizationCommand.h"

#include "gnomonVisualization/gnomonView/gnomonVtkView.h"
#include "gnomonVisualization/gnomonVisualizations/gnomonAbstractVtkVisualization.h"


void gnomonAbstractVtkVisualizationCommand::setView(gnomonAbstractView *view)
{
    if (auto vtk_view = dynamic_cast<gnomonVtkView *>(view)) {
        gnomonAbstractVisualizationCommand::setView(vtk_view);
    }
}

void gnomonAbstractVtkVisualizationCommand::clear(void)
{
    auto &&visu = std::static_pointer_cast<gnomonAbstractVtkVisualization>(this->visu);
    visu->disconnect();
    visu->clearConnections();
    gnomonAbstractVisualizationCommand::clear();
}