#include "gnomonAbstractCommand.h"
#include "gnomonAbstractCellImageFromImage.h"

class GNOMONCORE_EXPORT gnomonSegmentationCommand : public gnomonAbstractCommand<gnomonAbstractCellImageFromImage>
{
public:
     gnomonSegmentationCommand(void) = delete;
     gnomonSegmentationCommand(const QString&);
    ~gnomonSegmentationCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setImage(dtkImage *image);
    void setParameter(const QString& parameterName, const QVariant& parameterValue);

public:
    dtkImage *computedImage(void) const;

protected:
    using gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action;

private:
    class gnomonSegmentationCommandPrivate *d;
};
