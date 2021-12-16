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

gnomonBinaryImageFromImageCommand::gnomonBinaryImageFromImageCommand(void) : d(new gnomonBinaryImageFromImageCommandPrivate)
{
    this->factory_name = "binaryImageFromImage";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::binaryImageFromImage::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::binaryImageFromImage::pluginFactory().create(this->algorithm_name);
    }

}

gnomonBinaryImageFromImageCommand::~gnomonBinaryImageFromImageCommand(void)
{
    delete d;
}

void gnomonBinaryImageFromImageCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::binaryImageFromImage::pluginFactory().create(algo_name);
}

void gnomonBinaryImageFromImageCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonBinaryImageSeries *image = ((gnomonAbstractBinaryImageFromImage *) this->action)->output();
    if ((!image)||(image->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonBinaryImageFromImageCommand::undo(void)
{
    ((gnomonAbstractBinaryImageFromImage *) this->action)->setInput(nullptr);
    ((gnomonAbstractBinaryImageFromImage *) this->action)->setInitialization(nullptr);
}

void gnomonBinaryImageFromImageCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

dtkCoreParameters gnomonBinaryImageFromImageCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonBinaryImageFromImageCommand::setInput(gnomonImageSeries *image){
    if ((!image)||(image->times().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = image;
        Q_ASSERT(this->action);
        ((gnomonAbstractBinaryImageFromImage *) this->action)->setInput(d->input);
    }
}

gnomonImageSeries *gnomonBinaryImageFromImageCommand::input(void)
{
    return d->input;
}

void gnomonBinaryImageFromImageCommand::setInitialization(gnomonBinaryImageSeries *init)
{
    if ((!init)||(init->times().size()==0)) {
        d->initialization = nullptr;
    } else {
        d->initialization = init;
        Q_ASSERT(this->action);
        ((gnomonAbstractBinaryImageFromImage *) this->action)->setInitialization(d->initialization);
    }
}

gnomonBinaryImageSeries *gnomonBinaryImageFromImageCommand::initialization(void)
{
    return d->initialization;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonBinaryImageFromImageCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    inputs["initialization"] = this->initialization();
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonBinaryImageFromImageCommand::inputTypes(void)
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

gnomonBinaryImageSeries *gnomonBinaryImageFromImageCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonBinaryImageFromImageCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonBinaryImageFromImageCommand::outputTypes(void)
{
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonBinaryImage"));
    return output_types;
}

bool gnomonBinaryImageFromImageCommand::isEmpty(void)
{
    loadPluginGroup("binaryImageFromImage");
    return gnomonCore::binaryImageFromImage::pluginFactory().keys().size() == 0;
}

//
// gnomonBinaryImageFromImageCommand.cpp ends here
