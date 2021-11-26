#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageFromImageCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageFromImageCommand(void);
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
    virtual orderedMap inputTypes(void) override;
    virtual void setInputForm(const QString& name, gnomonAbstractDynamicForm *form) override;

public:
    virtual void setParameter(const QString&, const QVariant&);
    virtual void setAlgorithmName(const QString &) override;

    virtual dtkCoreParameters parameters(void) const override;

public:
    gnomonCellImageSeries *output();

    QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;
    orderedMap outputTypes(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageFromImageCommandPrivate *d;
};

//
// gnomonCellImageFromImageCommand.h ends here
