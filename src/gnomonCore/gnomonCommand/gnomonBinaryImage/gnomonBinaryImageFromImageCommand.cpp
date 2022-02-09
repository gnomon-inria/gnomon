#include "gnomonBinaryImageFromImageCommand.h"

#include <dtkScript>

class gnomonBinaryImageFromImageCommandPrivate
{
public:
    gnomonBinaryImageSeries* output = nullptr;
    gnomonImageSeries *input = nullptr;
    gnomonBinaryImageSeries* initialization = nullptr;
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
    gnomonBinaryImageSeries *image = ((gnomonAbstractBinaryImageFromImage *) this->action)->output();
    if ((!image)||(image->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonBinaryImageFromImageCommand::undo()
{
    ((gnomonAbstractBinaryImageFromImage *) this->action)->setInput(nullptr);
    ((gnomonAbstractBinaryImageFromImage *) this->action)->setInitialization(nullptr);
}

void gnomonBinaryImageFromImageCommand::setInput(gnomonImageSeries *image){
    if ((!image)||(image->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = image;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractBinaryImageFromImage *) this->action)->setInput(d->input);
}

gnomonImageSeries *gnomonBinaryImageFromImageCommand::input()
{
    return d->input;
}

void gnomonBinaryImageFromImageCommand::setInitialization(gnomonBinaryImageSeries *init)
{
    if ((!init)||(init->times().empty())) {
        d->initialization = nullptr;
    } else {
        d->initialization = init;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractBinaryImageFromImage *) this->action)->setInitialization(d->initialization);
}

gnomonBinaryImageSeries *gnomonBinaryImageFromImageCommand::initialization()
{
    return d->initialization;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonBinaryImageFromImageCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
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

void gnomonBinaryImageFromImageCommand::setInputForm(const QString& name, gnomonAbstractDynamicForm *form)
{
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonImageSeries *>(form));
    } else if (name == "initialization") {
        this->setInitialization(dynamic_cast<gnomonBinaryImageSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

gnomonBinaryImageSeries *gnomonBinaryImageFromImageCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonBinaryImageFromImageCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
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
        d->output = new gnomonBinaryImageSeries();
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
