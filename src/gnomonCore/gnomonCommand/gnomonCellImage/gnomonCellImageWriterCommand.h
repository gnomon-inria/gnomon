#include "gnomonCommand/gnomonAbstractCommand.h"

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
    void setCellImage(gnomonCellImageSeries *image_series);

private:
    class gnomonCellImageWriterCommandPrivate *d;
};
