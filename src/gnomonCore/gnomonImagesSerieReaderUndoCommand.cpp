#include "gnomonImagesSerieReaderUndoCommand.h"

gnomonImagesSerieReaderUndoCommand::gnomonImagesSerieReaderUndoCommand(const QString& key)
{
    gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command = gnomonCore::imagesSerieReader::pluginFactory().create(key);
}

void gnomonImagesSerieReaderUndoCommand::redo(void) {
    qDebug() << __LINE__ << Q_FUNC_INFO;
    qDebug() << "gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command" << gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command;
    Q_ASSERT(gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command);

    gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command->run();
}

void gnomonImagesSerieReaderUndoCommand::undo(void) {

    qDebug() << __LINE__ << Q_FUNC_INFO;
    qWarning() << "Does nothing";
}
