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

#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImagesSerie;
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"

class GNOMONCORE_EXPORT gnomonSegmentationCommand : public gnomonAbstractCommand
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
    gnomonCellImageSeries *output();
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonSegmentationCommandPrivate *d;
};

//
// gnomonSegmentationCommand.h ends here
