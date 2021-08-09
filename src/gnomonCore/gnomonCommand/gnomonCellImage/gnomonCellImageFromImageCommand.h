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
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"


class GNOMONCORE_EXPORT gnomonCellImageFromImageCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonCellImageFromImageCommand(void) = delete;
     gnomonCellImageFromImageCommand(const QString&);
    ~gnomonCellImageFromImageCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonImageSeries *image_series);
    gnomonImageSeries *input();

public:
    void setCellPoints(gnomonPointCloudSeries *pointCloud_series);
    gnomonPointCloudSeries *cellPoints();

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    void setParameter(const QString&, const QVariant&);
    virtual dtkCoreParameters parameters(void) const override;

public:
    gnomonCellImageSeries *output();
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageFromImageCommandPrivate *d;
};

//
// gnomonCellImageFromImageCommand.h ends here
