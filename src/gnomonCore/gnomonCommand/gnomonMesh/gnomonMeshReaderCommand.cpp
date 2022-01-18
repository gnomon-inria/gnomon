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

#include "gnomonMeshReaderCommand.h"

#include <dtkScript>

class gnomonMeshReaderCommandPrivate
{
public:
    gnomonMeshSeries *mesh = nullptr;
};

gnomonMeshReaderCommand::gnomonMeshReaderCommand() : d(new gnomonMeshReaderCommandPrivate)
{
    this->factory_name = "meshReader";
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
    }
}

gnomonMeshReaderCommand::~gnomonMeshReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonMeshReaderCommand::predo(void)
{
    ((gnomonAbstractMeshReader *) this->action)->setPath(this->m_path);
}

void gnomonMeshReaderCommand::postdo(void)
{
    gnomonMeshSeries *mesh = ((gnomonAbstractMeshReader *) this->action)->mesh();

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

gnomonMeshSeries *gnomonMeshReaderCommand::mesh()
{
    return d->mesh;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshReaderCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["mesh"] = this->mesh();
    return outputs;
}

bool gnomonMeshReaderCommand::isEmpty()
{
    loadPluginGroup("meshReader");
    return gnomonCore::meshReader::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonMeshReaderCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("mesh", "gnomonMesh"));
    return output_types;
}

//
// gnomonMeshReaderCommand.cpp ends here
