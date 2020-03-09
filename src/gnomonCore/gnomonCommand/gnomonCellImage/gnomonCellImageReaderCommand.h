#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageReaderCommand : public gnomonAbstractReaderCommand
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

    gnomonCellImageSeries *cellImage(void);

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageReaderCommandPrivate *d;
};
