#include "gnomonImagesSerieReaderUndoCommand.h"

#include <dtkScript>

class gnomonImagesSerieReaderUndoCommandPrivate
{
public:
    QString path;
};

gnomonImagesSerieReaderUndoCommand::gnomonImagesSerieReaderUndoCommand(const QString& key) : d(new gnomonImagesSerieReaderUndoCommandPrivate)
{
    QString command = "import " + key;

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command = gnomonCore::imagesSerieReader::pluginFactory().create(key);

    Q_ASSERT(gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command);
}

gnomonImagesSerieReaderUndoCommand::~gnomonImagesSerieReaderUndoCommand()
{
    delete d;
}

void gnomonImagesSerieReaderUndoCommand::redo(void)
{
    Q_ASSERT(gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command);
    gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command->setPath(d->path);
    gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command->run();
}

void gnomonImagesSerieReaderUndoCommand::undo(void)
{
    gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command->setPath("");
}

void gnomonImagesSerieReaderUndoCommand::setPath(const QString& path)
{
    d->path = path;
}

dtkImage *gnomonImagesSerieReaderUndoCommand::at(double t)
{
    return gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command->at(t);
}

dtkImage *gnomonImagesSerieReaderUndoCommand::next(void)
{
    return gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command->next();
}
