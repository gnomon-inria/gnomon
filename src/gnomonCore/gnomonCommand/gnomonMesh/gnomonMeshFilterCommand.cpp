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

#include "gnomonMeshFilterCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonMeshFilterCommandPrivate
{
public:
    gnomonMeshSeries* input = nullptr;
    gnomonMeshSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshFilterCommand::gnomonMeshFilterCommand(const QString& key) : d(new gnomonMeshFilterCommandPrivate)
{
    this->factory_name = "meshFilter";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::meshFilter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonMeshFilterCommand::~gnomonMeshFilterCommand(void)
{
    delete d;
}

void gnomonMeshFilterCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonMeshSeries *mesh = ((gnomonAbstractMeshFilter *) this->action)->output();
    if ((!mesh)||(mesh->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = mesh;
    }
}

void gnomonMeshFilterCommand::undo(void)
{
    ((gnomonAbstractMeshFilter *) this->action)->setInput(nullptr);
}

void gnomonMeshFilterCommand::setInput(gnomonMeshSeries *input)
{
    if ((!input)||(input->times().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractMeshFilter *) this->action)->setInput(d->input);
    }
}

void gnomonMeshFilterCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonMeshFilterCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonMeshSeries *gnomonMeshFilterCommand::input(void)
{
    return d->input;
}

gnomonMeshSeries *gnomonMeshFilterCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshFilterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshFilterCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonMeshFilterCommand::isEmpty(void)
{
    loadPluginGroup("meshFilter");
    return gnomonCore::meshFilter::pluginFactory().keys().size() == 0;
}

//
// gnomonMeshFilterCommand.cpp ends here
