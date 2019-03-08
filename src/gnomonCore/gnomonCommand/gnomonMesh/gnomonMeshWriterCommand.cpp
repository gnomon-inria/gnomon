#include "gnomonMeshWriterCommand.h"

#include <dtkScript>

class gnomonMeshWriterCommandPrivate
{
public:
    QString path;
    gnomonMesh* mesh = nullptr;
};

gnomonMeshWriterCommand::gnomonMeshWriterCommand(const QString& key) : d(new gnomonMeshWriterCommandPrivate)
{
    QString command = "import gnomonMeshWriter";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

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
    ((gnomonAbstractMeshWriter *) this->action)->setPath(d->path);
    ((gnomonAbstractMeshWriter *) this->action)->setMesh(d->mesh);
    this->action->run();
}

void gnomonMeshWriterCommand::undo(void)
{
    ((gnomonAbstractMeshWriter *) this->action)->setPath("");
}

void gnomonMeshWriterCommand::setPath(const QString& path)
{
    d->path = path;
}

void gnomonMeshWriterCommand::setMesh(gnomonMesh *mesh)
{
    d->mesh = mesh;
}