#include "gnomonAbstractCommand.h"
#include "gnomonAbstractMultiChannelImageReader.h"

class gnomonMultiChannelImage;
class GNOMONCORE_EXPORT gnomonMultiChannelImageReaderCommand : public gnomonAbstractCommand<gnomonAbstractMultiChannelImageReader>
{
public:
    gnomonMultiChannelImageReaderCommand() = delete;
    gnomonMultiChannelImageReaderCommand(const QString&);
    virtual ~gnomonMultiChannelImageReaderCommand();

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    gnomonMultiChannelImage *image(void);
    QStringList types(void);

private:
    class gnomonMultiChannelImageReaderCommandPrivate *d;
};
