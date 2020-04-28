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
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageFilterCommand::gnomonImageFilterCommand(const QString& key) : d(new gnomonImageFilterCommandPrivate)
{
    this->factory_name = "imageFilter";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::imageFilter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImageFilterCommand::~gnomonImageFilterCommand(void)
{
    delete d;
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

QMap<QString, gnomonCoreParameter *> gnomonImageFilterCommand::parameters(void) const
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
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageFilterCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonImageFilterCommand::isEmpty(void)
{
    loadPluginGroup("imageFilter");
    return gnomonCore::imageFilter::pluginFactory().keys().size() == 0;
}

//
// gnomonImageFilterCommand.cpp ends here
