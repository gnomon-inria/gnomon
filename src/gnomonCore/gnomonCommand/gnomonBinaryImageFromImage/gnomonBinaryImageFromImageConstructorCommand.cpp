#include "gnomonImageConstructorCommand.h"

#include <dtkScript>

class gnomonBinaryImageFromImageConstructorCommandPrivate
{
public:
    gnomonBinaryImageSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonBinaryImageFromImageConstructorCommand::gnomonBinaryImageFromImageConstructorCommand(void) : d(new gnomonBinaryImageFromImageConstructorCommandPrivate)
{
    this->factory_name = "binaryImageFromImageConstructor";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::binaryImageFromImageConstructor::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::binaryImageFromImageConstructor::pluginFactory().create(this->algorithm_name);
    }

}

gnomonBinaryImageFromImageConstructorCommand::~gnomonBinaryImageFromImageConstructorCommand(void)
{
    delete d;
}

void gnomonBinaryImageFromImageConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::binaryImageFromImageConstructor::pluginFactory().create(algo_name);
}

void gnomonBinaryImageFromImageConstructorCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonBinaryImageSeries *image = ((gnomonAbstractBinaryImageFromImageConstructor *) this->action)->output();
    if ((!image)||(image->times().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonBinaryImageFromImageConstructorCommand::undo(void)
{
}

void gnomonBinaryImageFromImageConstructorCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

dtkCoreParameters gnomonBinaryImageFromImageConstructorCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonBinaryImageSeries *gnomonBinaryImageFromImageConstructorCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonBinaryImageFromImageConstructorCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonBinaryImageFromImageConstructorCommand::isEmpty(void)
{
    loadPluginGroup("binaryImageFromImageConstructor");
    return gnomonCore::binaryImageFromImageConstructor::pluginFactory().keys().size() == 0;
}

//
// gnomonBinaryImageFromImageConstructorCommand.cpp ends here
