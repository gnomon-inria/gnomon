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

#include "gnomonImageConstructorCommand.h"

#include <dtkScript>

class gnomonImageConstructorCommandPrivate
{
public:
    gnomonImageSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageConstructorCommand::gnomonImageConstructorCommand(const QString& key) : d(new gnomonImageConstructorCommandPrivate)
{
    loadPluginGroup("imageConstructor");

    this->action = gnomonCore::imageConstructor::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImageConstructorCommand::~gnomonImageConstructorCommand(void)
{
    delete d;
}

void gnomonImageConstructorCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonImageConstructorCommand::undo(void)
{
}

void gnomonImageConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonImageConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImageSeries *gnomonImageConstructorCommand::output(void)
{
    gnomonImageSeries *image = ((gnomonAbstractImageConstructor *) this->action)->output();
    if ((!image)||(image->times().size()==0)) {
        return nullptr;
    } else {
        d->output = image;
        return image;
    }
}

//
// gnomonImageConstructorCommand.cpp ends here
