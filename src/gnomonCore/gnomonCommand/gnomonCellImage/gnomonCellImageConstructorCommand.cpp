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

#include "gnomonCellImageConstructorCommand.h"

#include <dtkScript>

class gnomonCellImageConstructorCommandPrivate
{
public:
    gnomonCellImageSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageConstructorCommand::gnomonCellImageConstructorCommand(const QString& key) : d(new gnomonCellImageConstructorCommandPrivate)
{
    loadPluginGroup("cellImageConstructor");

    this->action = gnomonCore::cellImageConstructor::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellImageConstructorCommand::~gnomonCellImageConstructorCommand(void)
{
    delete d;
}

void gnomonCellImageConstructorCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonCellImageConstructorCommand::undo(void)
{
}

void gnomonCellImageConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonCellImageConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImageSeries *gnomonCellImageConstructorCommand::output(void)
{
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageConstructor *) this->action)->output();
    if ((!cellImage)||(cellImage->times().size()==0)) {
        return nullptr;
    } else {
        d->output = cellImage;
        return cellImage;
    }
}

bool gnomonCellImageConstructorCommand::isEmpty(void)
{
    loadPluginGroup("cellImageConstructor");
    return gnomonCore::cellImageConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonCellImageConstructorCommand.cpp ends here
