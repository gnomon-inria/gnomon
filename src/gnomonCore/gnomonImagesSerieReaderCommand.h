#include "gnomonAbstractCommand.h"
#include "gnomonAbstractImagesSerieReader.h"

class GNOMONCORE_EXPORT gnomonImagesSerieReaderCommand : public gnomonAbstractCommand<gnomonAbstractImagesSerieReader>
{
public:
    gnomonImagesSerieReaderCommand() = delete;
    gnomonImagesSerieReaderCommand(const QString&);
    virtual ~gnomonImagesSerieReaderCommand();

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    dtkImage *image(const QString& channel = "");
    QStringList channels(void);

private:
    class gnomonImagesSerieReaderCommandPrivate *d;
};
