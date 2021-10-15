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

#include "gnomonCellImageFilterCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonCellImageFilterCommandPrivate
{
public:
    gnomonCellImageSeries* input = nullptr;
    gnomonCellImageSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageFilterCommand::gnomonCellImageFilterCommand(void) : d(new gnomonCellImageFilterCommandPrivate)
{
    this->factory_name = "cellImageFilter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellImageFilter::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellImageFilter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellImageFilterCommand::~gnomonCellImageFilterCommand(void)
{
    delete d;
}

void gnomonCellImageFilterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::cellImageFilter::pluginFactory().create(algo_name);
}

void gnomonCellImageFilterCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageFilter *) this->action)->output();
    if ((!cellImage)||(cellImage->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = cellImage;
    }
}

void gnomonCellImageFilterCommand::undo(void)
{
    ((gnomonAbstractCellImageFilter *) this->action)->setInput(nullptr);
}

void gnomonCellImageFilterCommand::setInput(gnomonCellImageSeries *input)
{
    if ((!input)||(input->times().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageFilter *) this->action)->setInput(d->input);
    }
}

void gnomonCellImageFilterCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

dtkCoreParameters gnomonCellImageFilterCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImageSeries *gnomonCellImageFilterCommand::input(void)
{
    return d->input;
}

gnomonCellImageSeries *gnomonCellImageFilterCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageFilterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageFilterCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonCellImageFilterCommand::isEmpty(void)
{
    loadPluginGroup("cellImageFilter");
    return gnomonCore::cellImageFilter::pluginFactory().keys().size() == 0;
}

//
// gnomonCellImageFilterCommand.cpp ends here
