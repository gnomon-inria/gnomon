#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageQuantificationCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonCellImageQuantificationCommand(void) = delete;
     gnomonCellImageQuantificationCommand(const QString&);
    ~gnomonCellImageQuantificationCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setImage(gnomonImageSeries *image);
    void setCellImage(gnomonCellImageSeries *cellimage);

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonCellImageSeries *cellImage(void);
    gnomonDataFrameSeries *dataFrame(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    dtkCoreParameters parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageQuantificationCommandPrivate *d;
};
