#include "gnomonAbstractVtkVisualizationCommand.h"

#include "gnomonVisualization/gnomonView/gnomonViewForm.h"
#include "gnomonVisualization/gnomonVisualizations/gnomonAbstractFormVisualization.h"

void gnomonAbstractVtkVisualizationCommand::setView(gnomonAbstractView *view)
{
    if (auto vtk_view = dynamic_cast<gnomonViewForm *>(view)) {
        this->_view = vtk_view;
    }
}