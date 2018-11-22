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

    virtual QMap<QString, gnomonParameter*> parameters(void) const;

    virtual void setParameter(const QString&, const QVariant&);

public:
    gnomonCellImage *computedImage(void) const;

private:
    class gnomonSegmentationCommandPrivate *d;
    using gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action;
};

//
// gnomonSegmentationCommand.h ends here
