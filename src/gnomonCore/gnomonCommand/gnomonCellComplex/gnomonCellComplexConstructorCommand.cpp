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

#include "gnomonCellComplexConstructorCommand.h"

#include <dtkScript>

class gnomonCellComplexConstructorCommandPrivate
{
public:
    gnomonCellComplexSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellComplexConstructorCommand::gnomonCellComplexConstructorCommand(const QString& key) : d(new gnomonCellComplexConstructorCommandPrivate)
{
    loadPluginGroup("cellComplexConstructor");

    this->action = gnomonCore::cellComplexConstructor::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellComplexConstructorCommand::~gnomonCellComplexConstructorCommand(void)
{
    delete d;
}

void gnomonCellComplexConstructorCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonCellComplexConstructorCommand::undo(void)
{
}

void gnomonCellComplexConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonCellComplexConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellComplexSeries *gnomonCellComplexConstructorCommand::output(void)
{
    gnomonCellComplexSeries *cellComplex = ((gnomonAbstractCellComplexConstructor *) this->action)->output();
    if ((!cellComplex)||(cellComplex->times().size()==0)) {
        return nullptr;
    } else {
        d->output = cellComplex;
        return cellComplex;
    }
}

bool gnomonCellComplexConstructorCommand::isEmpty(void)
{
    loadPluginGroup("cellComplexConstructor");
    return gnomonCore::cellComplexConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonCellComplexConstructorCommand.cpp ends here
