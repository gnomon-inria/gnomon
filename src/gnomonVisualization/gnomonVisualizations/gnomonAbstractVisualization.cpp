#include "gnomonAbstractVisualization.h"
#include "gnomonAbstractVisualization_p.h"

#include <dtkCore>


// /////////////////////////////////////////////////////////////////
// gnomonAbstractVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractVisualization::gnomonAbstractVisualization(void) : d(new gnomonAbstractVisualizationPrivate)
{
}

gnomonAbstractVisualization::~gnomonAbstractVisualization(void)
{
    delete d;
    d = nullptr;
}

void gnomonAbstractVisualization::setView(gnomonAbstractView* view)
{
    d->view = view;
}

gnomonAbstractView* gnomonAbstractVisualization::view(void)
{
    return d->view;
}

QVariantMap gnomonAbstractVisualization::visuParameters(void)
{
    QVariantMap parameters;

    dtkCoreParameters dtkParameters = this->parameters();
    for(const auto& param_name : dtkParameters.keys()){
        QVariant param_value = dtkParameters[param_name]->variant();
        parameters.insert(param_name, param_value);
    }
    return parameters;
}

void gnomonAbstractVisualization::setVisuParameters(QVariantMap parameters)
{
    for(const auto& param_name: parameters.keys()) {
        QVariant param = parameters[param_name];
        this->setParameter(param_name, param);
    }
}

void gnomonAbstractVisualization::connectParameter(const QString& parameter_name)
{
    dtkCoreParameters params = this->parameters();
    dtkCoreParameter *parameter = params.value(parameter_name, nullptr);
    if (parameter) {
        parameter->connect([=]() {
            this->onParameterChanged(parameter_name);
        });
    }
}

void gnomonAbstractVisualization::refreshParameters(void) {
    qDebug()<<Q_FUNC_INFO<<"Not implemented";
}

void gnomonAbstractVisualization::onParameterChanged(const QString& parameter_name) {
    qDebug()<<Q_FUNC_INFO<<"Not implemented";
}

//
// gnomonAbstractVisualization.cpp ends here
