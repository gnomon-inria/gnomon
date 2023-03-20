#include "gnomonAbstractVtkVisualizationCommand.h"

#include "gnomonVisualization/gnomonView/gnomonVtkView.h"
#include "gnomonVisualization/gnomonVisualizations/gnomonAbstractVtkVisualization.h"


void gnomonAbstractVtkVisualizationCommand::setAlgorithmName(const QString& visu_name)
{
    this->algorithm_name = visu_name;
    auto &&old_visu = std::static_pointer_cast<gnomonAbstractVtkVisualization>(this->visu);
    if (old_visu) {
        old_visu->clearConnections();
        old_visu->clear();
        old_visu->deleteLater();
    }
}

void gnomonAbstractVtkVisualizationCommand::setView(gnomonAbstractView *view)
{
    if (auto vtk_view = dynamic_cast<gnomonVtkView *>(view)) {
        this->_view = vtk_view;
    }
}

void gnomonAbstractVtkVisualizationCommand::setVisualizationParameters(const QVariantMap &parameters)
{
    auto &&visu = std::static_pointer_cast<gnomonAbstractVtkVisualization>(this->visu);
    visu->clearConnections();
    visu->clear();
    visu->setView(this->_view);
    visu->setVisuParameters(parameters);
    visu->update();
    visu->setVisible(true);
}