#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class GNOMONCORE_EXPORT gnomonImageWriterCommand : public gnomonAbstractCommand
{
public:
     gnomonImageWriterCommand(void) = delete;
     gnomonImageWriterCommand(const QString&);
    ~gnomonImageWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    void setImage(gnomonImageSeries *images_serie);

private:
    class gnomonImageWriterCommandPrivate *d;
};
