#include "gnomonImageFilterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageFilter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageFilterCommandPrivate
{
public:
    std::shared_ptr<gnomonImageSeries> input;
    std::shared_ptr<gnomonImageSeries> output;
    std::shared_ptr<gnomonBinaryImageSeries> mask;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageFilterCommand::gnomonImageFilterCommand() : d(new gnomonImageFilterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonImageFilterCommand)

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

void gnomonImageFilterCommand::predo(void)
{
    this->action->is_async = true;
}

void gnomonImageFilterCommand::postdo(void)
{
    std::shared_ptr<gnomonImageSeries> image = ((gnomonAbstractImageFilter *) this->action)->output();

    if ((!image)||(image->times().empty())||(image->current()->channels().empty())) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonImageFilterCommand::undo()
{
    ((gnomonAbstractImageFilter *) this->action)->setInput(nullptr);
    ((gnomonAbstractImageFilter *) this->action)->setMask(nullptr);
    this->action->refreshParameters();
}

void gnomonImageFilterCommand::setInput(std::shared_ptr<gnomonImageSeries> input)
{
    if ((!input)||(input->times().empty())||(input->current()->channels().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractImageFilter *) this->action)->setInput(d->input);
    ((gnomonAbstractImageFilter *) this->action)->refreshParameters();
}

std::shared_ptr<gnomonImageSeries> gnomonImageFilterCommand::input()
{
    return d->input;
}

std::shared_ptr<gnomonImageSeries> gnomonImageFilterCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonImageFilterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
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

void gnomonImageFilterCommand::setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else if(name == "mask") {
        this->setMask(std::dynamic_pointer_cast<gnomonBinaryImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonImageFilterCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonImageFilterCommand::outputTypes()
{
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonImage"));
    return output_types;
}

void gnomonImageFilterCommand::setMask(std::shared_ptr<gnomonBinaryImageSeries> init)
{
    if ((!init)||(init->times().size()==0)) {
        d->mask = nullptr;
    } else {
        d->mask = init;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractImageFilter *) this->action)->setMask(d->mask);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonBinaryImageSeries> gnomonImageFilterCommand::mask(void)
{
    return d->mask;
}

void gnomonImageFilterCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonImageSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonImageFilterCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

// gnomonImageFilterCommand.cpp ends here
