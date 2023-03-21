#include "gnomonAbstractVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonAbstractVisualization>

gnomonAbstractView *gnomonAbstractVisualizationCommand::view(void)
{
    return this->_view;
}

void gnomonAbstractVisualizationCommand::setView(gnomonAbstractView *view)
{
    this->_view = view;
}

std::shared_ptr<gnomonAbstractVisualization> gnomonAbstractVisualizationCommand::visualization(void)
{
    return this->visu;
}

void gnomonAbstractVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setAlgorithmName(visu_name);
    this->visu->setView(this->_view);
    this->setVisualizationParameters(parameters);
}

void gnomonAbstractVisualizationCommand::setVisualizationParameters(const QVariantMap &parameters)
{
    this->visu->setVisuParameters(parameters);
    this->visu->setVisible(this->visible);
}

QVariantMap gnomonAbstractVisualizationCommand::visualizationParameters(void)
{
    return this->visu->visuParameters();
}

void gnomonAbstractVisualizationCommand::update(void)
{
    this->visu->update();
    this->visu->setVisible(this->visible);
}

void gnomonAbstractVisualizationCommand::setVisible(bool visible)
{
    this->visible = visible;
    this->visu->setVisible(this->visible);
}