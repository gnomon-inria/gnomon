#include "gnomonAbstractCommand.h"
#include "gnomonAbstractCellImageFromImage.h"

class GNOMONCORE_EXPORT gnomonSegmentationCommand : public gnomonAbstractCommand<gnomonAbstractCellImageFromImage>
{
public:
    gnomonSegmentationCommand() = delete;
    gnomonSegmentationCommand(const QString&);
    virtual ~gnomonSegmentationCommand();

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setImage(dtkImage *image);
    void setParameter(const QString& parameterName, const QVariant& parameterValue);

public:
    dtkImage *computedImage();

private:
    class gnomonSegmentationCommandPrivate *d;
};
