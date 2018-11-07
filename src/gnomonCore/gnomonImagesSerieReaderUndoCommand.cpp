#include "gnomonImagesSerieReaderUndoCommand.h"

#include <dtkScript>

class gnomonImagesSerieReaderUndoCommandPrivate
{
public:
    QString path;
};

gnomonImagesSerieReaderUndoCommand::gnomonImagesSerieReaderUndoCommand(const QString& key) : d(new gnomonImagesSerieReaderUndoCommandPrivate)
{
    dtkScriptInterpreterPython *interpreter = dtkScriptInterpreterPython::instance();

    QString command = "import gnomonImagesSerieReader";

    int stat;
    interpreter->interpret(command, &stat);

    Q_ASSERT(stat == 0);

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
    qWarning() << "Does nothing";
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
