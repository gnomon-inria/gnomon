#include "gnomonAbstractCommand.h"
#include "gnomonAbstractImagesSerieReader.h"

class GNOMONCORE_EXPORT gnomonImagesSerieReaderCommand : public gnomonAbstractCommand
{
public:
     gnomonImagesSerieReaderCommand(void) = delete;
     gnomonImagesSerieReaderCommand(const QString&);
    ~gnomonImagesSerieReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    gnomonImagesSerie *imagesSerie();

private:
    class gnomonImagesSerieReaderCommandPrivate *d;
};
