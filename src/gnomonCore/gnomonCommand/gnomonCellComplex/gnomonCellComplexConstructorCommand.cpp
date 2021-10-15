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

gnomonCellComplexConstructorCommand::gnomonCellComplexConstructorCommand(void) : d(new gnomonCellComplexConstructorCommandPrivate)
{
    this->factory_name = "cellComplexConstructor";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellComplexConstructor::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellComplexConstructor::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellComplexConstructorCommand::~gnomonCellComplexConstructorCommand(void)
{
    delete d;
}

void gnomonCellComplexConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::cellComplexConstructor::pluginFactory().create(algo_name);
}


void gnomonCellComplexConstructorCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellComplexSeries *cellComplex = ((gnomonAbstractCellComplexConstructor *) this->action)->output();
    if ((!cellComplex)||(cellComplex->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = cellComplex;
    }
}

void gnomonCellComplexConstructorCommand::undo(void)
{
}

void gnomonCellComplexConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

dtkCoreParameters gnomonCellComplexConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellComplexSeries *gnomonCellComplexConstructorCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexConstructorCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonCellComplexConstructorCommand::isEmpty(void)
{
    loadPluginGroup("cellComplexConstructor");
    return gnomonCore::cellComplexConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonCellComplexConstructorCommand.cpp ends here
