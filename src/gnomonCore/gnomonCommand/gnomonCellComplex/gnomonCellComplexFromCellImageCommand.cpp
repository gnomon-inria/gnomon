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

#include "gnomonCellComplexFromCellImageCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonCellComplexFromCellImageCommandPrivate
{
public:
    gnomonCellImageSeries *input = nullptr;
    gnomonCellComplexSeries *output = nullptr;
};

gnomonCellComplexFromCellImageCommand::gnomonCellComplexFromCellImageCommand(const QString& key) : d(new gnomonCellComplexFromCellImageCommandPrivate)
{
    this->factory_name = "cellComplexFromCellImage";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::cellComplexFromCellImage::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellComplexFromCellImageCommand::~gnomonCellComplexFromCellImageCommand(void)
{
    delete d;
}

void gnomonCellComplexFromCellImageCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellComplexSeries *cellComplex = ((gnomonAbstractCellComplexFromCellImage *) this->action)->output();
    if ((!cellComplex)||(cellComplex->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = cellComplex;
    }
}

void gnomonCellComplexFromCellImageCommand::undo(void)
{
    ((gnomonAbstractCellComplexFromCellImage *) this->action)->setInput(nullptr);
}

void gnomonCellComplexFromCellImageCommand::setInput(gnomonCellImageSeries *input)
{
    if ((!input)||(input->times().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellComplexFromCellImage *) this->action)->setInput(d->input);
    }
}

void gnomonCellComplexFromCellImageCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, dtkCoreParameter *> gnomonCellComplexFromCellImageCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImageSeries *gnomonCellComplexFromCellImageCommand::input(void)
{
    return d->input;
}

gnomonCellComplexSeries *gnomonCellComplexFromCellImageCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexFromCellImageCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexFromCellImageCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonCellComplexFromCellImageCommand::isEmpty(void)
{
    loadPluginGroup("cellComplexFromCellImage");
    return gnomonCore::cellComplexFromCellImage::pluginFactory().keys().size() == 0;
}

//
// gnomonCellComplexFromCellImageCommand.cpp ends here
