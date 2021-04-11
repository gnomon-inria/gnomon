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

#include "gnomonFormAlgorithmCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonFormAlgorithmCommandPrivate
{
public:
    gnomonImageSeries* inputImage = nullptr;

    gnomonImageSeries* outputImage = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonFormAlgorithmCommand::gnomonFormAlgorithmCommand(const QString& key) : d(new gnomonFormAlgorithmCommandPrivate)
{
    this->factory_name = "formAlgorithm";

    this->algorithm_name = key;
    this->action = gnomonCore::formAlgorithm::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonFormAlgorithmCommand::~gnomonFormAlgorithmCommand(void)
{
    delete d;
}

void gnomonFormAlgorithmCommand::redo(void)
{
}

void gnomonFormAlgorithmCommand::undo(void)
{
}

void gnomonFormAlgorithmCommand::addInput(gnomonAbstractDynamicForm *input)
{
    if (gnomonImageSeries* image = dynamic_cast<gnomonImageSeries *>(input)) {
        d->inputImage = image;
    }
}

void gnomonFormAlgorithmCommand::addOutput(gnomonAbstractDynamicForm *output)
{
    if (gnomonImageSeries* image = dynamic_cast<gnomonImageSeries *>(output)) {
        d->outputImage = image;
    }
}

void gnomonFormAlgorithmCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonFormAlgorithmCommand::parameters(void) const
{
    return this->action->parameters();
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonFormAlgorithmCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    if (d->inputImage) {
        inputs["inputImage"] = d->inputImage;
    }
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonFormAlgorithmCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    if (d->outputImage) {
        outputs["outputImage"] = d->outputImage;
    }
    return outputs;
}


//
// gnomonFormAlgorithmCommand.cpp ends here
