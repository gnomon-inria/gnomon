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

class gnomonImagesSerie;
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
    void setInput(gnomonImagesSerie *images_serie);
    gnomonImagesSerie *input();
    
    void setParameter(const QString&, const QVariant&);

public:
    gnomonImagesSerie *output();
    QMap<QString, gnomonCoreParameter *> parameters(void) const;
    gnomonCellImage *computedImage(void) const;

private:
    class gnomonSegmentationCommandPrivate *d;
    using gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action;
};

//
// gnomonSegmentationCommand.h ends here
