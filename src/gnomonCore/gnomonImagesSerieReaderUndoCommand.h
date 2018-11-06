#include "gnomonAbstractUndoCommand.h"
#include "gnomonAbstractImagesSerieReader.h"

class GNOMONCORE_EXPORT gnomonImagesSerieReaderUndoCommand : public gnomonAbstractUndoCommand<gnomonAbstractImagesSerieReader>
{
public:
    gnomonImagesSerieReaderUndoCommand() = delete;
    gnomonImagesSerieReaderUndoCommand(const QString&);
    virtual ~gnomonImagesSerieReaderUndoCommand();

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    dtkImage *at(double t);
    dtkImage *next(void);

private:
    class gnomonImagesSerieReaderUndoCommandPrivate *d;
};
