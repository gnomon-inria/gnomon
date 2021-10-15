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

gnomonMeshReaderCommand::gnomonMeshReaderCommand(void) : d(new gnomonMeshReaderCommandPrivate)
{
    this->factory_name = "meshReader";
    loadPluginGroup(this->factoryName());

    for (auto key: gnomonCore::imageReader::pluginFactory().keys()) {
        auto algo = gnomonCore::imageReader::pluginFactory().create(key);
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

void gnomonMeshReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractMeshReader *) this->action)->setPath(this->m_path);
    this->action->run();
    gnomonMeshSeries *mesh = ((gnomonAbstractMeshReader *) this->action)->mesh();
    if ((!mesh)||(mesh->times().size()==0)) {
        d->mesh = nullptr;
    } else {
        d->mesh = mesh;
    }
}

void gnomonMeshReaderCommand::undo(void)
{
    ((gnomonAbstractMeshReader *) this->action)->setPath("");
}

void gnomonMeshReaderCommand::setPath(const QString& path)
{
    this->m_path = path;
}

gnomonMeshSeries *gnomonMeshReaderCommand::mesh(void)
{
    return d->mesh;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshReaderCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["mesh"] = this->mesh();
    return outputs;
}

bool gnomonMeshReaderCommand::isEmpty(void)
{
    loadPluginGroup("meshReader");
    return gnomonCore::meshReader::pluginFactory().keys().size() == 0;
}

//
// gnomonMeshReaderCommand.cpp ends here
