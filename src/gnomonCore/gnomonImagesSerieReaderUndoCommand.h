#include "gnomonAbstractUndoCommand.h"
#include "gnomonAbstractImagesSerieReader.h"

class GNOMONCORE_EXPORT gnomonImagesSerieReaderUndoCommand : public gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>
{
public:
    gnomonImagesSerieReaderUndoCommand() = delete;
    gnomonImagesSerieReaderUndoCommand(const QString&);

public:
    void redo(void) override;
    void undo(void) override;
};
