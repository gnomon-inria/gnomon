#include "gnomonAbstractVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonAbstractVisualization>


void gnomonAbstractVisualizationCommand::setAlgorithmName(const QString& visu_name)
{
    if ((this->algorithm_name != visu_name) || (this->visu == nullptr)) {
        this->algorithm_name = visu_name;
        this->newVisualization();
    }
}

gnomonAbstractView *gnomonAbstractVisualizationCommand::view(void)
{
    return this->_view;
}

void gnomonAbstractVisualizationCommand::setView(gnomonAbstractView *view)
{
    this->_view = view;
    if (this->visu) {
        this->visu->setView(this->_view);
    }
}

std::shared_ptr<gnomonAbstractVisualization> gnomonAbstractVisualizationCommand::visualization(void)
{
    return this->visu;
}

void gnomonAbstractVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setAlgorithmName(visu_name);
    if (this->visu) {
        this->visu->setView(this->_view);
    }
    this->setVisualizationParameters(parameters);
}

void gnomonAbstractVisualizationCommand::setVisualizationParameters(const QVariantMap &parameters)
{
    if (this->visu) {
        this->visu->setVisuParameters(parameters);
        this->visu->setVisible(this->visible);
    }
}

QVariantMap gnomonAbstractVisualizationCommand::visualizationParameters(void)
{
    QVariantMap parameters;
    if (this->visu) {
        parameters = this->visu->visuParameters();
    }
    return parameters;
}

dtkCoreParameters gnomonAbstractVisualizationCommand::parameters(void) const
{
    dtkCoreParameters parameters;
    if (this->visu) {
        parameters = this->visu->parameters();
    }
    return parameters;
}

void gnomonAbstractVisualizationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    if (this->visu) {
        this->visu->setParameter(parameter, value);
    }
}

QMap<QString, QString> gnomonAbstractVisualizationCommand::parameterGroups() const
{
    QMap<QString, QString> groups;
    if (this->visu) {
        groups = this->visu->parameterGroups();
    }
    return groups;
}

void gnomonAbstractVisualizationCommand::update(void)
{
    if (this->visu) {
        this->visu->update();
        this->visu->setVisible(this->visible);
    }
}

void gnomonAbstractVisualizationCommand::clear(void)
{
    this->disconnectVisualization();
    if (this->visu) {
        this->visu->clear();
        this->visu = nullptr;
    }
}

void gnomonAbstractVisualizationCommand::setVisible(bool visible)
{
    this->visible = visible;
    if (this->visu) {
        this->visu->setVisible(this->visible);
    }
}

void gnomonAbstractVisualizationCommand::setVisualization(std::shared_ptr<gnomonAbstractVisualization> visu)
{
    // Replace visualization by existing (shared) visualization (e.g. on form drop from manager)
    if (visu && visu != this->visu) {
        // Remove current visualization from the view
        this->clear();
        this->visu = visu;
        this->algorithm_name = this->visu->pluginName();
        // Update the view of the visualization to make sure it is displayed
        this->connectVisualization();
    }
}

void gnomonAbstractVisualizationCommand::connectVisualization(void)
{
    if (this->visu) {
        this->visu->setView(this->_view);
        connect(visu.get(), &gnomonAbstractVisualization::parametersChanged, [=]() {
            emit visuParametersChanged();
        });
    }
}

void gnomonAbstractVisualizationCommand::disconnectVisualization(void)
{
    if (this->visu) {
        this->visu->disconnect();
        this->visu->clearConnections();
    }
}