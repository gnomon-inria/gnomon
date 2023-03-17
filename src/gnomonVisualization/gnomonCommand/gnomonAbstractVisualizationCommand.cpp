#include "gnomonAbstractVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonAbstractVisualization>

void setVisuParameters(std::shared_ptr<gnomonAbstractVisualization> visu, QVariantMap parameters)
{
    for(const auto& param_name: parameters.keys()) {
        QVariant param = parameters[param_name];
        visu->setParameter(param_name, param);
    }
};

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
}