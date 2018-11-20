// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonAbstractCommand.h"
#include "gnomonAbstractCellImageFromImage.h"

class gnomonCellImage;

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
    virtual QMap<QString, QVariant> parameters(void) const;

public:
    gnomonCellImage *computedImage(void) const;

protected:
    using gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action;

private:
    class gnomonSegmentationCommandPrivate *d;
};

//
// gnomonSegmentationCommand.h ends here
