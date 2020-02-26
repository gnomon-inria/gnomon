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
    void setInput(gnomonImageSeries *image_series);
    gnomonImageSeries *input();

public:
    void setCellPoints(gnomonPointCloudSeries *pointCloud_series);
    gnomonPointCloudSeries *cellPoints();

public:
    void setParameter(const QString&, const QVariant&);

public:
    gnomonCellImageSeries *output();
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonSegmentationCommandPrivate *d;
};

//
// gnomonSegmentationCommand.h ends here
