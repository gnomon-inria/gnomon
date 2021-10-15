#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageTrackingCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonCellImageTrackingCommand(void);
    ~gnomonCellImageTrackingCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setImage(gnomonImageSeries *image);
    void setCellImage(gnomonCellImageSeries *cellimage);

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonCellImageSeries *cellImage(void);
    gnomonTreeSeries *tree(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);
    void setAlgorithmName(const QString &) override;

public:
    dtkCoreParameters parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageTrackingCommandPrivate *d;
};
