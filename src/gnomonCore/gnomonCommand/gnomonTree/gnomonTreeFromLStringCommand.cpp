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

#include "gnomonTreeFromLStringCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonTreeFromLStringCommandPrivate
{
public:
    gnomonLStringSeries *lstring = nullptr;

//public:
//    QString lsystem;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeFromLStringCommand::gnomonTreeFromLStringCommand(const QString& key) : d(new gnomonTreeFromLStringCommandPrivate)
{
    loadPluginGroup("treeFromLString");

    this->action = gnomonCore::treeFromLString::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonTreeFromLStringCommand::~gnomonTreeFromLStringCommand()
{
    delete d;
}

void gnomonTreeFromLStringCommand::redo(void)
{
    Q_ASSERT(this->action);
//    ((gnomonAbstractTreeFromLString *) this->action)->setLSystem(d->lsystem);
    this->action->run();
}

void gnomonTreeFromLStringCommand::undo(void)
{
//    ((gnomonAbstractTreeFromLString *) this->action)->setLSystem("");
}

//void gnomonTreeFromLStringCommand::setLSystem(const QString& lsystem)
//{
//    d->lsystem = lsystem;
//}

void gnomonTreeFromLStringCommand::setInput(gnomonLStringSeries* lstring)
{
    d->lstring = lstring;
    ((gnomonAbstractTreeFromLString *) this->action)->setInput(d->lstring);
}

gnomonLStringSeries *gnomonTreeFromLStringCommand::input()
{
    gnomonLStringSeries *lString = ((gnomonAbstractTreeFromLString *) this->action)->input();
    if ((!lString)||(lString->times().size()==0)) {
        return nullptr;
    } else {
        return lString;
    }
}

gnomonTreeSeries *gnomonTreeFromLStringCommand::output()
{
    gnomonTreeSeries *tree = ((gnomonAbstractTreeFromLString *) this->action)->output();
    if ((!tree)||(tree->times().size()==0)) {
        return nullptr;
    } else {
        return tree;
    }
}

QMap<QString, gnomonCoreParameter *> gnomonTreeFromLStringCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonTreeFromLStringCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

bool gnomonTreeFromLStringCommand::isEmpty(void)
{
    loadPluginGroup("treeFromLString");
    return gnomonCore::treeFromLString::pluginFactory().keys().size() == 0;
}

//
// gnomonTreeFromLStringCommand.cpp ends here
