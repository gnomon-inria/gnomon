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

#include "gnomonMeshFromImageCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonMeshFromImageCommandPrivate
{
public:
    gnomonImageSeries *input = nullptr;
    gnomonMeshSeries *output = nullptr;
};

gnomonMeshFromImageCommand::gnomonMeshFromImageCommand(const QString& key) : d(new gnomonMeshFromImageCommandPrivate)
{
    this->factory_name = "meshFromImage";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::meshFromImage::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonMeshFromImageCommand::~gnomonMeshFromImageCommand(void)
{
    delete d;
}

void gnomonMeshFromImageCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonMeshSeries *mesh = ((gnomonAbstractMeshFromImage *) this->action)->output();
    if ((!mesh)||(mesh->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = mesh;
    }
}

void gnomonMeshFromImageCommand::undo(void)
{
    ((gnomonAbstractMeshFromImage *) this->action)->setInput(nullptr);
}

void gnomonMeshFromImageCommand::setInput(gnomonImageSeries *input)
{
    if ((!input)||(input->times().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractMeshFromImage *) this->action)->setInput(d->input);
    }
}

void gnomonMeshFromImageCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonMeshFromImageCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImageSeries *gnomonMeshFromImageCommand::input(void)
{
    return d->input;
}

gnomonMeshSeries *gnomonMeshFromImageCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshFromImageCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshFromImageCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonMeshFromImageCommand::isEmpty(void)
{
    loadPluginGroup("meshFromImage");
    return gnomonCore::meshFromImage::pluginFactory().keys().size() == 0;
}

//
// gnomonMeshFromImageCommand.cpp ends here
