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
    this->factory_name = "cellImageConstructor";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;

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

    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageConstructor *) this->action)->output();
    if ((!cellImage)||(cellImage->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = cellImage;
    }
}

void gnomonCellImageConstructorCommand::undo(void)
{
}

void gnomonCellImageConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

dtkCoreParameters gnomonCellImageConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImageSeries *gnomonCellImageConstructorCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageConstructorCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonCellImageConstructorCommand::isEmpty(void)
{
    loadPluginGroup("cellImageConstructor");
    return gnomonCore::cellImageConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonCellImageConstructorCommand.cpp ends here
