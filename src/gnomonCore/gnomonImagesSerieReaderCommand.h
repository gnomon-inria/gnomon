#include "gnomonAbstractCommand.h"
#include "gnomonAbstractImagesSerieReader.h"

class GNOMONCORE_EXPORT gnomonImagesSerieReaderCommand : public gnomonAbstractCommand<gnomonAbstractImagesSerieReader>
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

    double time(void);
    dtkImage *at(double t, const QString& channel = QString());
    dtkImage *next(void);
    QStringList channels(void);

private:
    class gnomonImagesSerieReaderCommandPrivate *d;
    using gnomonAbstractCommand<gnomonAbstractImagesSerieReader>::action;
};
