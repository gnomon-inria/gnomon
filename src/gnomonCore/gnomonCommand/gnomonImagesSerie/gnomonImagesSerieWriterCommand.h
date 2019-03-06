#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImagesSerie;

class GNOMONCORE_EXPORT gnomonImagesSerieWriterCommand : public gnomonAbstractCommand
{
public:
     gnomonImagesSerieWriterCommand(void) = delete;
     gnomonImagesSerieWriterCommand(const QString&);
    ~gnomonImagesSerieWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    void setImagesSerie(gnomonImagesSerie *images_serie);

private:
    class gnomonImagesSerieWriterCommandPrivate *d;
};
