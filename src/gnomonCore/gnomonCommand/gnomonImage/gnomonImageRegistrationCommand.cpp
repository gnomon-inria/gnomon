// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonImageRegistrationCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageRegistrationCommandPrivate
{
public:
    QVector<gnomonImageSeries *> images_series;

    gnomonImageSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageRegistrationCommand::gnomonImageRegistrationCommand(void) : d(new gnomonImageRegistrationCommandPrivate)
{
    this->factory_name = "imageRegistration";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::imageRegistration::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::imageRegistration::pluginFactory().create(this->algorithm_name);
    }
}

gnomonImageRegistrationCommand::~gnomonImageRegistrationCommand()
{
    delete d;
}

void gnomonImageRegistrationCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::imageRegistration::pluginFactory().create(algo_name);
}

void gnomonImageRegistrationCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonImageSeries *image = ((gnomonAbstractImageRegistration *) this->action)->output();
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonImageRegistrationCommand::undo(void)
{
    d->images_series.clear();
}

void gnomonImageRegistrationCommand::addImage(gnomonImageSeries *image_series)
{
    d->images_series.push_back(image_series);

    ((gnomonAbstractImageRegistration *) this->action)->removeImages();
    for(auto& image_series : d->images_series) {
        ((gnomonAbstractImageRegistration *) this->action)->addImage(image_series);
    };
}

gnomonImageSeries* gnomonImageRegistrationCommand::output()
{
    return d->output;
}

dtkCoreParameters gnomonImageRegistrationCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonImageRegistrationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageRegistrationCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    int input_count = 1;
    for(auto& image_series : d->images_series) {
        QString input_name = "image" + QString::number(input_count);
        inputs[input_name] = image_series;
        input_count++;
    }
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageRegistrationCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonImageRegistrationCommand::isEmpty(void)
{
    loadPluginGroup("imageRegistration");
    return gnomonCore::imageRegistration::pluginFactory().keys().size() == 0;
}

//
// gnomonImageRegistrationCommand.cpp ends here
