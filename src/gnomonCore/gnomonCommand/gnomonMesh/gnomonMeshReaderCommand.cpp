#include "gnomonMeshReaderCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshReader.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonMeshReaderCommandPrivate
{
public:
    std::shared_ptr<gnomonMeshSeries> mesh = nullptr;
};

gnomonMeshReaderCommand::gnomonMeshReaderCommand() : d(new gnomonMeshReaderCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    for (const auto& key: gnomonCore::meshReader::pluginFactory().keys()) {
        auto algo = gnomonCore::meshReader::pluginFactory().create(key);
        if (!this->action) {
            this->action = algo;
            this->algorithm_name = key;
        }
        m_descriptions.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
        m_preview.insert(key, algo->preview());
    }
}

gnomonMeshReaderCommand::~gnomonMeshReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonMeshReaderCommand::predo(void)
{
    this->action->is_async = true;
    ((gnomonAbstractMeshReader *) this->action)->setPath(this->m_path);
}

void gnomonMeshReaderCommand::postdo(void)
{
    std::shared_ptr<gnomonMeshSeries> mesh = ((gnomonAbstractMeshReader *) this->action)->mesh();

    if ((!mesh)||(mesh->times().empty())) {
        d->mesh = nullptr;
    } else {
        d->mesh = mesh;
    }
}

void gnomonMeshReaderCommand::undo()
{
    ((gnomonAbstractMeshReader *) this->action)->setPath("");
}

std::shared_ptr<gnomonMeshSeries> gnomonMeshReaderCommand::mesh()
{
    return d->mesh;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonMeshReaderCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["mesh"] = this->mesh();
    return outputs;
}

bool gnomonMeshReaderCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonMeshReaderCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonMeshReaderCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("mesh", "gnomonMesh"));
    return output_types;
}

void gnomonMeshReaderCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->mesh) {
        d->mesh = std::make_shared<gnomonMeshSeries>();
    }
    auto tmp = serialization["mesh"].toObject();
    d->mesh->deserialize(tmp);
}

QJsonObject gnomonMeshReaderCommand::serializeResults(void) {
    QJsonObject out;
    out["mesh"] = d->mesh->serialize();
    return out;
}

GNOMON_REGISTER_TYPE(gnomonMeshReaderCommand)

//
// gnomonMeshReaderCommand.cpp ends here
