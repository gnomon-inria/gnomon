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

#include "gnomonTreeTransformCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonTreeTransformCommandPrivate
{
public:
    gnomonTreeSeries* input = nullptr;
    gnomonTreeSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeTransformCommand::gnomonTreeTransformCommand(const QString& key) : d(new gnomonTreeTransformCommandPrivate)
{
    this->factory_name = "treeTransform";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::treeTransform::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonTreeTransformCommand::~gnomonTreeTransformCommand(void)
{
    delete d;
}

void gnomonTreeTransformCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonTreeSeries *tree = ((gnomonAbstractTreeTransform *) this->action)->output();
    if ((!tree)||(tree->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = tree;
    }
}

void gnomonTreeTransformCommand::undo(void)
{
    ((gnomonAbstractTreeTransform *) this->action)->setInput(nullptr);
}

void gnomonTreeTransformCommand::setInput(gnomonTreeSeries *input)
{
    if ((!input)||(input->times().size()==0)) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractTreeTransform *) this->action)->setInput(d->input);
    }
}

void gnomonTreeTransformCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, dtkCoreParameter *> gnomonTreeTransformCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonTreeSeries *gnomonTreeTransformCommand::input(void)
{
    return d->input;
}

gnomonTreeSeries *gnomonTreeTransformCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeTransformCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeTransformCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonTreeTransformCommand::isEmpty(void)
{
    loadPluginGroup("treeTransform");
    return gnomonCore::treeTransform::pluginFactory().keys().size() == 0;
}

//
// gnomonTreeTransformCommand.cpp ends here
