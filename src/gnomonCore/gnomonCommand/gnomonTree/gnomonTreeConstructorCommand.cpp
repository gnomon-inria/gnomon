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

#include "gnomonTreeConstructorCommand.h"

#include <dtkScript>

class gnomonTreeConstructorCommandPrivate
{
public:
    gnomonTreeSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeConstructorCommand::gnomonTreeConstructorCommand(const QString& key) : d(new gnomonTreeConstructorCommandPrivate)
{
    loadPluginGroup("treeConstructor");

    this->action = gnomonCore::treeConstructor::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonTreeConstructorCommand::~gnomonTreeConstructorCommand(void)
{
    delete d;
}

void gnomonTreeConstructorCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonTreeConstructorCommand::undo(void)
{
}

void gnomonTreeConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonTreeConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonTreeSeries *gnomonTreeConstructorCommand::output(void)
{
    gnomonTreeSeries *tree = ((gnomonAbstractTreeConstructor *) this->action)->output();
    if ((!tree)||(tree->times().size()==0)) {
        return nullptr;
    } else {
        d->output = tree;
        return tree;
    }
}

bool gnomonTreeConstructorCommand::isEmpty(void)
{
    loadPluginGroup("treeConstructor");
    return gnomonCore::treeConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonTreeConstructorCommand.cpp ends here
