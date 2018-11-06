#include "gnomonImagesSerieReaderUndoCommand.h"

gnomonImagesSerieReaderUndoCommand::gnomonImagesSerieReaderUndoCommand(const QString& key)
{
    gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command = gnomonCore::imagesSerieReader::pluginFactory().create(key);
}

void gnomonImagesSerieReaderUndoCommand::redo(void) {
    Q_ASSERT(gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command);

    gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command->run();
}

void gnomonImagesSerieReaderUndoCommand::undo(void) {
    qWarning() << "Does nothing";
}
