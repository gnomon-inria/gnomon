#include "gnomonImageRegistrationCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageRegistrationCommandPrivate
{
public:
    gnomonAbstractCommand::orderedMap input_types = {{"reference", "gnomonImage"}, {"input", "gnomonImage"}};
    QMap<QString, gnomonAbstractDynamicForm *> inputs = {{"reference", nullptr}, {"input", nullptr}};
    //QVector<gnomonImageSeries *> images_series;

    gnomonImageSeries* output = nullptr; //TODO do same thing as for input with second member a dataDict
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
    d->inputs["reference"] = nullptr;
    d->inputs["input"] = nullptr;
}

void gnomonImageRegistrationCommand::addImage(gnomonImageSeries *image_series)
{
    //first set reference, then set input
    if (d->inputs["reference"] == nullptr) {
        d->inputs["reference"] = image_series;
    } else if (d->inputs["input"] == nullptr) {
        d->inputs["input"] = image_series;

        ((gnomonAbstractImageRegistration *) this->action)->removeImages();
        ((gnomonAbstractImageRegistration *) this->action)->addImage(static_cast<gnomonImageSeries *>(d->inputs["reference"]));
        ((gnomonAbstractImageRegistration *) this->action)->addImage(image_series);
    } else {
        dtkWarn() << Q_FUNC_INFO << "reference and input are already set. Do a undo/clear before. I will do nothing.";
    }

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
    return d->inputs;
}

gnomonAbstractCommand::orderedMap gnomonImageRegistrationCommand::inputTypes(void)
{
    return d->input_types;
}

void gnomonImageRegistrationCommand::addInputForm(gnomonAbstractDynamicForm *form)
{
    gnomonImageSeries *image = dynamic_cast<gnomonImageSeries *>(form);
    if (image) {
        this->addImage(image);
    } else {
        dtkWarn() << Q_FUNC_INFO << "cannot cast form to gnomonImageSeries, bad input: " << form;
    }
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageRegistrationCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonImageRegistrationCommand::outputTypes(void)
{
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonImage"));
    return output_types;
}

bool gnomonImageRegistrationCommand::isEmpty(void)
{
    loadPluginGroup("imageRegistration");
    return gnomonCore::imageRegistration::pluginFactory().keys().size() == 0;
}

//
// gnomonImageRegistrationCommand.cpp ends here
