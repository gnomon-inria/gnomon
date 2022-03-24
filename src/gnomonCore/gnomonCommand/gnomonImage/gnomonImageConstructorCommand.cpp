#include "gnomonImageConstructorCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageConstructor.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonImageConstructorCommandPrivate
{
public:
    gnomonImageSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageConstructorCommand::gnomonImageConstructorCommand() : d(new gnomonImageConstructorCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::imageConstructor::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::imageConstructor::pluginFactory().create(this->algorithm_name);
    }

}

gnomonImageConstructorCommand::~gnomonImageConstructorCommand()
{
    delete d;
}

void gnomonImageConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::imageConstructor::pluginFactory().create(algo_name);
}

void gnomonImageConstructorCommand::predo(void)
{

}

void gnomonImageConstructorCommand::postdo(void)
{
    gnomonImageSeries *image = ((gnomonAbstractImageConstructor *) this->action)->output();

    if ((!image)||(image->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonImageConstructorCommand::undo()
{
}

gnomonImageSeries *gnomonImageConstructorCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageConstructorCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonImageConstructorCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonImageConstructorCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonImageConstructorCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonImage"));
    return types;
}

void gnomonImageConstructorCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = new gnomonImageSeries();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonImageConstructorCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

//
// gnomonImageConstructorCommand.cpp ends here
