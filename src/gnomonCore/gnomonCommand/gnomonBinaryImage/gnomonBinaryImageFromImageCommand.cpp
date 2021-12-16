#include "gnomonBinaryImageFromImageCommand.h"

#include <dtkScript>

class gnomonBinaryImageFromImageCommandPrivate
{
public:
    gnomonBinaryImageSeries* output = nullptr;
    gnomonImageSeries *input = nullptr;

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

QMap<QString, gnomonAbstractDynamicForm *> gnomonBinaryImageFromImageCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
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

gnomonBinaryImageSeries *gnomonBinaryImageFromImageCommand::initialization(void)
{
    d->output = nullptr;
    Q_ASSERT(this->action);
    ((gnomonAbstractBinaryImageFromImage *) this->action)->setInput(d->input);
    return d->output;
}

bool gnomonBinaryImageFromImageCommand::isEmpty(void)
{
    loadPluginGroup("binaryImageFromImage");
    return gnomonCore::binaryImageFromImage::pluginFactory().keys().size() == 0;
}

//
// gnomonBinaryImageFromImageCommand.cpp ends here
