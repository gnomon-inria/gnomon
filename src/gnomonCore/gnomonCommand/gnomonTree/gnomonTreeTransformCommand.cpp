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
    return ((gnomonAbstractTreeTransform *) this->action)->input();
}

gnomonTreeSeries *gnomonTreeTransformCommand::output(void)
{
    return ((gnomonAbstractTreeTransform *) this->action)->output();
}

//
// gnomonTreeTransformCommand.cpp ends here
