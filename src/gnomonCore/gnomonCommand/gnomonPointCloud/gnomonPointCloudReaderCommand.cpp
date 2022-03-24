#include "gnomonPointCloudReaderCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudReader.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonPointCloudReaderCommandPrivate
{
public:
    gnomonPointCloudSeries *pointCloud = nullptr;
};

gnomonPointCloudReaderCommand::gnomonPointCloudReaderCommand() : d(new gnomonPointCloudReaderCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    for (const auto& key: gnomonCore::pointCloudReader::pluginFactory().keys()) {
        auto algo = gnomonCore::pointCloudReader::pluginFactory().create(key);
        if (!this->action) {
            this->action = algo;
            this->algorithm_name = key;
        }
        m_descriptions.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
    }
}

gnomonPointCloudReaderCommand::~gnomonPointCloudReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonPointCloudReaderCommand::predo(void)
{
    ((gnomonAbstractPointCloudReader *) this->action)->setPath(this->m_path);
}

void gnomonPointCloudReaderCommand::postdo(void)
{
    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudReader *) this->action)->pointCloud();

    if ((!pointCloud)||(pointCloud->times().empty())) {
        d->pointCloud = nullptr;
    } else {
        d->pointCloud = pointCloud;
    }
}

void gnomonPointCloudReaderCommand::undo()
{
    ((gnomonAbstractPointCloudReader *) this->action)->setPath("");
}

gnomonPointCloudSeries *gnomonPointCloudReaderCommand::pointCloud()
{
    return d->pointCloud;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudReaderCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["pointCloud"] = this->pointCloud();
    return outputs;
}

bool gnomonPointCloudReaderCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonPointCloudReaderCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonPointCloudReaderCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("pointCloud", "gnomonPointCloud"));
    return output_types;
}

void gnomonPointCloudReaderCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->pointCloud) {
        d->pointCloud = new gnomonPointCloudSeries();
    }
    auto tmp = serialization["pointCloud"].toObject();
    d->pointCloud->deserialize(tmp);
}

QJsonObject gnomonPointCloudReaderCommand::serializeResults(void) {
    QJsonObject out;
    out["pointCloud"] = d->pointCloud->serialize();
    return out;
}
//
// gnomonPointCloudReaderCommand.cpp ends here
