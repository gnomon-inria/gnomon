#include "gnomonPointCloudConstructorCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudConstructor.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonPointCloudConstructorCommandPrivate
{
public:
    std::shared_ptr<gnomonPointCloudSeries> output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonPointCloudConstructorCommand::gnomonPointCloudConstructorCommand() : d(new gnomonPointCloudConstructorCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::pointCloudConstructor::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::pointCloudConstructor::pluginFactory().create(this->algorithm_name);
    }
}

gnomonPointCloudConstructorCommand::~gnomonPointCloudConstructorCommand()
{
    delete d;
}

void gnomonPointCloudConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::pointCloudConstructor::pluginFactory().create(algo_name);
}

void gnomonPointCloudConstructorCommand::predo(void)
{

}

void gnomonPointCloudConstructorCommand::postdo(void)
{
    std::shared_ptr<gnomonPointCloudSeries> pointCloud = ((gnomonAbstractPointCloudConstructor *) this->action)->output();

    if ((!pointCloud)||(pointCloud->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = pointCloud;
    }
}

void gnomonPointCloudConstructorCommand::undo()
{
}

std::shared_ptr<gnomonPointCloudSeries> gnomonPointCloudConstructorCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonPointCloudConstructorCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonPointCloudConstructorCommand::isEmpty(){
    return availablePlugins().empty();
}

QStringList gnomonPointCloudConstructorCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonPointCloudConstructorCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonPointCloud"));
    return output_types;
}

void gnomonPointCloudConstructorCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonPointCloudSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonPointCloudConstructorCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

//
// gnomonPointCloudConstructorCommand.cpp ends here
