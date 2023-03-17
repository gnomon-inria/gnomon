#include "gnomonAbstractVtkVisualizationCommand.h"

#include "gnomonVisualization/gnomonView/gnomonViewForm.h"
#include "gnomonVisualization/gnomonVisualizations/gnomonAbstractFormVisualization.h"

void gnomonAbstractVtkVisualizationCommand::setView(gnomonAbstractView *view)
{
    if (auto vtk_view = dynamic_cast<gnomonViewForm *>(view)) {
        this->_view = vtk_view;
    }
}

void gnomonAbstractVtkVisualizationCommand::setVisualizationParameters(const QVariantMap &parameters)
{
    auto &&visu = std::static_pointer_cast<gnomonAbstractFormVisualization>(this->visu);
    visu->clearConnections();
    visu->clear();
    visu->setView(this->_view);
    visu->setVisuParameters(parameters);
    visu->update();
    visu->setVisible(true);
}