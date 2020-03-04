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
    QString path;
};

gnomonMeshReaderCommand::gnomonMeshReaderCommand(const QString& key) : d(new gnomonMeshReaderCommandPrivate)
{
    loadPluginGroup("meshReader");

    this->action = gnomonCore::meshReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonMeshReaderCommand::~gnomonMeshReaderCommand()
{
    delete d;
}

void gnomonMeshReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractMeshReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonMeshReaderCommand::undo(void)
{
    ((gnomonAbstractMeshReader *) this->action)->setPath("");
}

void gnomonMeshReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonMeshSeries *gnomonMeshReaderCommand::mesh(void)
{
    gnomonMeshSeries *mesh = ((gnomonAbstractMeshReader *) this->action)->mesh();
    if ((!mesh)||(mesh->times().size()==0)) {
        return nullptr;
    } else {
        return mesh;
    }
}

bool gnomonMeshReaderCommand::isEmpty(void)
{
    loadPluginGroup("meshReader");
    return gnomonCore::meshReader::pluginFactory().keys().size() == 0;
}

//
// gnomonMeshReaderCommand.cpp ends here
