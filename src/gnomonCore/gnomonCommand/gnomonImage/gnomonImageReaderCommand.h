#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonImageReaderCommand : public gnomonAbstractCommand
{
public:
     gnomonImageReaderCommand(void) = delete;
     gnomonImageReaderCommand(const QString&);
    ~gnomonImageReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonImageSeries *image(void);

private:
    class gnomonImageReaderCommandPrivate *d;
};
