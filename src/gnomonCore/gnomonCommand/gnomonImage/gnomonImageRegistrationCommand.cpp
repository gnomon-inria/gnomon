#include "gnomonImageRegistrationCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageRegistrationCommandPrivate
{
public:
    gnomonAbstractCommand::orderedMap input_types = {{"reference", "gnomonImage"}, {"input", "gnomonImage"}};
    gnomonAbstractCommand::orderedMap output_types = {{"output", "gnomonImage"}, {"transformation", "gnomonDataDict"}};

    QMap<QString, gnomonAbstractDynamicForm *> inputs = {{"reference", nullptr}, {"input", nullptr}};
    QMap<QString, gnomonAbstractDynamicForm *> outputs = {{"output", nullptr}, {"transformation", nullptr}};
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageRegistrationCommand::gnomonImageRegistrationCommand() : d(new gnomonImageRegistrationCommandPrivate)
{
    this->factory_name = "imageRegistration";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::imageRegistration::pluginFactory().keys();
    if (!keys.empty()) {
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

        delete this->action;
    this->action = gnomonCore::imageRegistration::pluginFactory().create(algo_name);
}

void gnomonImageRegistrationCommand::redo()
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonImageSeries *image = ((gnomonAbstractImageRegistration *) this->action)->output();
    if ((!image)||(image->times().empty())||(((gnomonImage *)image->current())->channels().empty())) {
        d->outputs["output"] = nullptr;
    } else {
        d->outputs["output"] = image;
    }

    gnomonDataDictSeries *transformation = ((gnomonAbstractImageRegistration *) this->action)->outputTransformation();
    if ((!transformation)||(transformation->times().empty())||(((gnomonDataDict *)transformation->current())->keys().empty())) {
        d->outputs["transformation"] = nullptr;
    } else {
        d->outputs["transformation"] = transformation;
    }
}

void gnomonImageRegistrationCommand::undo()
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
        ((gnomonAbstractImageRegistration *) this->action)->addImage(dynamic_cast<gnomonImageSeries *>(d->inputs["reference"]));
        ((gnomonAbstractImageRegistration *) this->action)->addImage(image_series);
    } else {
        dtkWarn() << Q_FUNC_INFO << "reference and input are already set. Do a undo/clear before. I will do nothing.";
    }

}

gnomonImageSeries* gnomonImageRegistrationCommand::output()
{
    return (gnomonImageSeries *)d->outputs["output"];
}


QMap<QString, gnomonAbstractDynamicForm *> gnomonImageRegistrationCommand::inputs()
{
    return d->inputs;
}

gnomonAbstractCommand::orderedMap gnomonImageRegistrationCommand::inputTypes()
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

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageRegistrationCommand::outputs()
{
    return d->outputs;
}

gnomonAbstractCommand::orderedMap gnomonImageRegistrationCommand::outputTypes()
{
    return d->output_types;
}

bool gnomonImageRegistrationCommand::isEmpty()
{
    loadPluginGroup("imageRegistration");
    return gnomonCore::imageRegistration::pluginFactory().keys().empty();
}

void gnomonImageRegistrationCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    // TODO: come back later to check if correct
    if(name == "reference") {
        d->inputs["reference"] = form;
    } else if(name == "input") {
        d->inputs["input"] = form;
    } else {
        dtkWarn() << Q_FUNC_INFO << "unknown input " << name;
        return;
    }

    ((gnomonAbstractImageRegistration *) this->action)->removeImages();
    ((gnomonAbstractImageRegistration *) this->action)->addImage(dynamic_cast<gnomonImageSeries *>(d->inputs["reference"]));
    ((gnomonAbstractImageRegistration *) this->action)->addImage(dynamic_cast<gnomonImageSeries *>(d->inputs["input"]));
}

//
// gnomonImageRegistrationCommand.cpp ends here
