#include "gnomonImagesSerieReaderUndoCommand.h"

void gnomonImagesSerieReaderUndoCommand::redo(void) {
    gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>::command->run();
}

void gnomonImagesSerieReaderUndoCommand::undo(void) {
    qWarning() << "Does nothing";
}
