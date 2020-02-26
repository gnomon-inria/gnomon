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
    gnomonTreeSeries *tree = ((gnomonAbstractLStringFromTree *) this->action)->input();
    if ((!tree)||(tree->times().size()==0)) {
        return nullptr;
    } else {
        return tree;
    }
}

gnomonLStringSeries *gnomonLStringFromTreeCommand::output()
{
    gnomonLStringSeries *lString = ((gnomonAbstractLStringFromTree *) this->action)->output();
    if ((!lString)||(lString->times().size()==0)) {
        return nullptr;
    } else {
        return lString;
    }
}

QMap<QString, gnomonCoreParameter *> gnomonLStringFromTreeCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonLStringFromTreeCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

bool gnomonLStringFromTreeCommand::isEmpty(void)
{
    loadPluginGroup("lStringFromTree");
    return gnomonCore::lStringFromTree::pluginFactory().keys().size() == 0;
}

//
// gnomonLStringFromTreeCommand.cpp ends here
