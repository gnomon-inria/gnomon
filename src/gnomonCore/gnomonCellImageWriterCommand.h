#include "gnomonAbstractCommand.h"
#include "gnomonAbstractCellImageWriter.h"

class GNOMONCORE_EXPORT gnomonCellImageWriterCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageWriterCommand(void) = delete;
     gnomonCellImageWriterCommand(const QString&);
    ~gnomonCellImageWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    void setCellImage(gnomonCellImage *images_serie);

private:
    class gnomonCellImageWriterCommandPrivate *d;
};
