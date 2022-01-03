#include "gnomonImageFilterCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageFilterCommandPrivate
{
public:
    gnomonImageSeries* input = nullptr;
    gnomonImageSeries* output = nullptr;
    gnomonBinaryImageSeries* mask = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageFilterCommand::gnomonImageFilterCommand(void) : d(new gnomonImageFilterCommandPrivate)
{
    this->factory_name = "imageFilter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::imageFilter::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::imageFilter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonImageFilterCommand::~gnomonImageFilterCommand(void)
{
    delete d;
}

void gnomonImageFilterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::imageFilter::pluginFactory().create(algo_name);
}

void gnomonImageFilterCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonImageSeries *image = ((gnomonAbstractImageFilter *) this->action)->output();
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonImageFilterCommand::undo(void)
{
    ((gnomonAbstractImageFilter *) this->action)->setInput(nullptr);
    ((gnomonAbstractImageFilter *) this->action)->setMask(nullptr);
}

void gnomonImageFilterCommand::setInput(gnomonImageSeries *input)
{
    if ((!input)||(input->times().size()==0)||(((gnomonImage *)input->current())->channels().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractImageFilter *) this->action)->setInput(d->input);
    }
}

void gnomonImageFilterCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

dtkCoreParameters gnomonImageFilterCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImageSeries *gnomonImageFilterCommand::input(void)
{
    return d->input;
}

gnomonImageSeries *gnomonImageFilterCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageFilterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    inputs["mask"] = this->mask();
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonImageFilterCommand::inputTypes(void)
{
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonImage"));
    input_types.emplace_back(std::make_pair("mask", "gnomonBinaryImage"));
    return input_types;
}

void gnomonImageFilterCommand::setInputForm(const QString& name, gnomonAbstractDynamicForm *form)
{
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonImageSeries *>(form));
    } else if(name == "mask") {
        this->setMask(dynamic_cast<gnomonBinaryImageSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonImageFilterCommand::addInputForm(gnomonAbstractDynamicForm *form) {
    this->setInputForm("input", form);
}


QMap<QString, gnomonAbstractDynamicForm *> gnomonImageFilterCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonImageFilterCommand::outputTypes(void)
{
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonImage"));
    return output_types;
}

bool gnomonImageFilterCommand::isEmpty(void)
{
    loadPluginGroup("imageFilter");
    return gnomonCore::imageFilter::pluginFactory().keys().size() == 0;
}

void gnomonImageFilterCommand::setMask(gnomonBinaryImageSeries *init)
{
    if ((!init)||(init->times().size()==0)) {
        d->mask = nullptr;
    } else {
        d->mask = init;
        Q_ASSERT(this->action);
        ((gnomonAbstractImageFilter *) this->action)->setMask(d->mask);
    }

}

gnomonBinaryImageSeries *gnomonImageFilterCommand::mask(void)
{
    return d->mask;
}

// gnomonImageFilterCommand.cpp ends here
