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

gnomonImageFilterCommand::gnomonImageFilterCommand() : d(new gnomonImageFilterCommandPrivate)
{
    this->factory_name = "imageFilter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::imageFilter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::imageFilter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonImageFilterCommand::~gnomonImageFilterCommand()
{
    delete d;
}

void gnomonImageFilterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::imageFilter::pluginFactory().create(algo_name);
}

void gnomonImageFilterCommand::redo()
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonImageSeries *image = ((gnomonAbstractImageFilter *) this->action)->output();
    if ((!image)||(image->times().empty())||(((gnomonImage *)image->current())->channels().empty())) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonImageFilterCommand::undo()
{
    ((gnomonAbstractImageFilter *) this->action)->setInput(nullptr);
    ((gnomonAbstractImageFilter *) this->action)->setMask(nullptr);
}

void gnomonImageFilterCommand::setInput(gnomonImageSeries *input)
{
    if ((!input)||(input->times().empty())||(((gnomonImage *)input->current())->channels().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractImageFilter *) this->action)->setInput(d->input);
    }
}

gnomonImageSeries *gnomonImageFilterCommand::input()
{
    return d->input;
}

gnomonImageSeries *gnomonImageFilterCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageFilterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    inputs["mask"] = this->mask();
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonImageFilterCommand::inputTypes()
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


QMap<QString, gnomonAbstractDynamicForm *> gnomonImageFilterCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonImageFilterCommand::outputTypes()
{
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonImage"));
    return output_types;
}

bool gnomonImageFilterCommand::isEmpty()
{
    loadPluginGroup("imageFilter");
    return gnomonCore::imageFilter::pluginFactory().keys().empty();
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
