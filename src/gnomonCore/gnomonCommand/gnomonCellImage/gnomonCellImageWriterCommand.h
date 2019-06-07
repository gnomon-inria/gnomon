#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"

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
    void setCellImage(gnomonCellImageSeries *images_serie);

private:
    class gnomonCellImageWriterCommandPrivate *d;
};
