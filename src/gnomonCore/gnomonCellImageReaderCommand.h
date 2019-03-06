#include "gnomonAbstractCommand.h"
class gnomonCellImage;

class GNOMONCORE_EXPORT gnomonCellImageReaderCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageReaderCommand(void) = delete;
     gnomonCellImageReaderCommand(const QString&);
    ~gnomonCellImageReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonCellImage *cellImage(void);

private:
    class gnomonCellImageReaderCommandPrivate *d;
};
