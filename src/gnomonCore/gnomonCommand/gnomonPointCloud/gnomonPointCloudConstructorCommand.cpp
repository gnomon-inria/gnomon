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

#include "gnomonPointCloudConstructorCommand.h"

#include <dtkScript>

class gnomonPointCloudConstructorCommandPrivate
{
public:
    gnomonPointCloudSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonPointCloudConstructorCommand::gnomonPointCloudConstructorCommand() : d(new gnomonPointCloudConstructorCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeFromLString::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeFromLString::pluginFactory().create(this->algorithm_name);
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
    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudConstructor *) this->action)->output();

    if ((!pointCloud)||(pointCloud->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = pointCloud;
    }
}

void gnomonPointCloudConstructorCommand::undo()
{
}

gnomonPointCloudSeries *gnomonPointCloudConstructorCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudConstructorCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
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

//
// gnomonPointCloudConstructorCommand.cpp ends here
