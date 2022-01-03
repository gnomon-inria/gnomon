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

gnomonCellComplexFromCellImageCommand::gnomonCellComplexFromCellImageCommand() : d(new gnomonCellComplexFromCellImageCommandPrivate)
{
    this->factory_name = "cellComplexFromCellImage";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellComplexFromCellImage::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellComplexFromCellImage::pluginFactory().create(this->algorithm_name);
    }

}

gnomonCellComplexFromCellImageCommand::~gnomonCellComplexFromCellImageCommand()
{
    delete d;
}

void gnomonCellComplexFromCellImageCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellComplexFromCellImage::pluginFactory().create(algo_name);
}

void gnomonCellComplexFromCellImageCommand::redo()
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellComplexSeries *cellComplex = ((gnomonAbstractCellComplexFromCellImage *) this->action)->output();
    if ((!cellComplex)||(cellComplex->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = cellComplex;
    }
}

void gnomonCellComplexFromCellImageCommand::undo()
{
    ((gnomonAbstractCellComplexFromCellImage *) this->action)->setInput(nullptr);
}

void gnomonCellComplexFromCellImageCommand::setInput(gnomonCellImageSeries *input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellComplexFromCellImage *) this->action)->setInput(d->input);
    }
}

gnomonCellImageSeries *gnomonCellComplexFromCellImageCommand::input()
{
    return d->input;
}

gnomonCellComplexSeries *gnomonCellComplexFromCellImageCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexFromCellImageCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexFromCellImageCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonCellComplexFromCellImageCommand::isEmpty()
{
    loadPluginGroup("cellComplexFromCellImage");
    return gnomonCore::cellComplexFromCellImage::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonCellComplexFromCellImageCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonCellImage"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonCellComplexFromCellImageCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonCellComplex"));
    return types;
}

//
// gnomonCellComplexFromCellImageCommand.cpp ends here
