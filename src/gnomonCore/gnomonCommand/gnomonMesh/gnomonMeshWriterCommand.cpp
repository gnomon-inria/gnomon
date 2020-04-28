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

#include "gnomonMeshWriterCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonMeshWriterCommandPrivate
{
public:
    gnomonMeshSeries* mesh = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshWriterCommand::gnomonMeshWriterCommand(const QString& key) : d(new gnomonMeshWriterCommandPrivate)
{
    this->factory_name = "meshWriter";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::meshWriter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonMeshWriterCommand::~gnomonMeshWriterCommand()
{
    delete d;
}

void gnomonMeshWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractMeshWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractMeshWriter *) this->action)->setMesh(d->mesh);
    this->action->run();
}

void gnomonMeshWriterCommand::undo(void)
{
    ((gnomonAbstractMeshWriter *) this->action)->setPath("");
}

void gnomonMeshWriterCommand::setPath(const QString& path)
{
    this->m_path = path;
}

void gnomonMeshWriterCommand::setMesh(gnomonMeshSeries *mesh)
{
    d->mesh = mesh;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshWriterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["mesh"] = d->mesh;
    return inputs;
}

bool gnomonMeshWriterCommand::isEmpty(void)
{
    loadPluginGroup("meshWriter");
    return gnomonCore::meshWriter::pluginFactory().keys().size() == 0;
}

//
// gnomonMeshWriterCommand.cpp ends here
