#include "gnomonBinaryImageFromImageCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonBinaryImage/gnomonAbstractBinaryImageFromImage.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>
#include <gnomonCore/gnomonCore.h>

#include <dtkScript>

class gnomonBinaryImageFromImageCommandPrivate
{
public:
    std::shared_ptr<gnomonBinaryImageSeries> output;
    std::shared_ptr<gnomonImageSeries> input;
    std::shared_ptr<gnomonBinaryImageSeries> initialization;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonBinaryImageFromImageCommand::gnomonBinaryImageFromImageCommand() : d(new gnomonBinaryImageFromImageCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::binaryImageFromImage::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::binaryImageFromImage::pluginFactory().create(this->algorithm_name);
    }
}

gnomonBinaryImageFromImageCommand::~gnomonBinaryImageFromImageCommand()
{
    delete d;
}

void gnomonBinaryImageFromImageCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

    delete this->action;
    this->action = gnomonCore::binaryImageFromImage::pluginFactory().create(algo_name);
}

void gnomonBinaryImageFromImageCommand::predo(void) {}

void gnomonBinaryImageFromImageCommand::postdo(void)
{
    std::shared_ptr<gnomonBinaryImageSeries> image = ((gnomonAbstractBinaryImageFromImage *) this->action)->output();
    if ((!image)||(image->times().empty())) {
        d->output.reset();
    } else {
        d->output = image;
    }
}

void gnomonBinaryImageFromImageCommand::undo()
{
    ((gnomonAbstractBinaryImageFromImage *) this->action)->setInput(nullptr);
    ((gnomonAbstractBinaryImageFromImage *) this->action)->setInitialization(nullptr);
    this->action->refreshParameters();
}

void gnomonBinaryImageFromImageCommand::setInput(std::shared_ptr<gnomonImageSeries> image){
    if ((!image)||(image->times().empty())) {
        d->input.reset();
    } else {
        d->input = image;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractBinaryImageFromImage *) this->action)->setInput(d->input);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonImageSeries> gnomonBinaryImageFromImageCommand::input()
{
    return d->input;
}

void gnomonBinaryImageFromImageCommand::setInitialization(std::shared_ptr<gnomonBinaryImageSeries> init)
{
    if ((!init)||(init->times().empty())) {
        d->initialization.reset();
    } else {
        d->initialization = init;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractBinaryImageFromImage *) this->action)->setInitialization(d->initialization);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonBinaryImageSeries> gnomonBinaryImageFromImageCommand::initialization()
{
    return d->initialization;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonBinaryImageFromImageCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["input"] = this->input();
    inputs["initialization"] = this->initialization();
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonBinaryImageFromImageCommand::inputTypes()
{
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonImage"));
    input_types.emplace_back(std::make_pair("initialization", "gnomonBinaryImage"));
    return input_types;
}

void gnomonBinaryImageFromImageCommand::setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else if (name == "initialization") {
        this->setInitialization(std::dynamic_pointer_cast<gnomonBinaryImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

std::shared_ptr<gnomonBinaryImageSeries> gnomonBinaryImageFromImageCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonBinaryImageFromImageCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonBinaryImageFromImageCommand::outputTypes()
{
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonBinaryImage"));
    return output_types;
}

bool gnomonBinaryImageFromImageCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonBinaryImageFromImageCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

void gnomonBinaryImageFromImageCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonBinaryImageSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonBinaryImageFromImageCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

//
// gnomonBinaryImageFromImageCommand.cpp ends here
