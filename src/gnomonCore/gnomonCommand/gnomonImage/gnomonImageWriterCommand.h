#include "gnomonCommand/gnomonAbstractCommand.h"

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
    void setImage(gnomonImageSeries *image_series);

private:
    class gnomonImageWriterCommandPrivate *d;
};
