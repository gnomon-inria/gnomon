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
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeTransformCommand::gnomonTreeTransformCommand(const QString& key) : d(new gnomonTreeTransformCommandPrivate)
{
    loadPluginGroup("treeTransform");

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
}

void gnomonTreeTransformCommand::undo(void)
{
    ((gnomonAbstractTreeTransform *) this->action)->setInput(nullptr);
}

void gnomonTreeTransformCommand::setInput(gnomonTreeSeries *input)
{
    d->input = input;

    Q_ASSERT(this->action);
    ((gnomonAbstractTreeTransform *) this->action)->setInput(d->input);
}

void gnomonTreeTransformCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonTreeTransformCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonTreeSeries *gnomonTreeTransformCommand::input(void)
{
    gnomonTreeSeries *tree = ((gnomonAbstractTreeTransform *) this->action)->input();
    if ((!tree)||(tree->times().size()==0)) {
        return nullptr;
    } else {
        return tree;
    }
}

gnomonTreeSeries *gnomonTreeTransformCommand::output(void)
{
    gnomonTreeSeries *tree = ((gnomonAbstractTreeTransform *) this->action)->output();
    if ((!tree)||(tree->times().size()==0)) {
        return nullptr;
    } else {
        return tree;
    }
}

//
// gnomonTreeTransformCommand.cpp ends here
