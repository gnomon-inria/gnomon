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

#include "gnomonLStringFromTreeCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonLStringFromTreeCommandPrivate
{
public:
    gnomonTreeSeries *tree_serie = nullptr;
};

gnomonLStringFromTreeCommand::gnomonLStringFromTreeCommand(const QString& key) : d(new gnomonLStringFromTreeCommandPrivate)
{
    loadPluginGroup("lStringFromTree");

    this->action = gnomonCore::lStringFromTree::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonLStringFromTreeCommand::~gnomonLStringFromTreeCommand(void)
{
    delete d;
}

void gnomonLStringFromTreeCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonLStringFromTreeCommand::undo(void)
{
    Q_ASSERT(this->action);

    ((gnomonAbstractLStringFromTree *) this->action)->setInput(nullptr);
}

void gnomonLStringFromTreeCommand::setInput(gnomonTreeSeries* tree_serie)
{
    d->tree_serie = tree_serie;
    ((gnomonAbstractLStringFromTree *) this->action)->setInput(d->tree_serie);
}

gnomonTreeSeries *gnomonLStringFromTreeCommand::input()
{
    return ((gnomonAbstractLStringFromTree *) this->action)->input();
}

gnomonLStringSeries *gnomonLStringFromTreeCommand::output()
{
    return ((gnomonAbstractLStringFromTree *) this->action)->output();
}

QMap<QString, gnomonCoreParameter *> gnomonLStringFromTreeCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonLStringFromTreeCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

//
// gnomonLStringFromTreeCommand.cpp ends here
