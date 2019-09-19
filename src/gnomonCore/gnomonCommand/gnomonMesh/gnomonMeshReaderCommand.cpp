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
    QString command = "import gnomonMeshReader";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

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
    return ((gnomonAbstractMeshReader *) this->action)->mesh();
}

//
// gnomonMeshReaderCommand.cpp ends here
