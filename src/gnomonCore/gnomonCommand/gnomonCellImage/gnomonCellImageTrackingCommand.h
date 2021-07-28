#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageTrackingCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonCellImageTrackingCommand(void) = delete;
     gnomonCellImageTrackingCommand(const QString&);
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

public:
    QMap<QString, dtkCoreParameter *> parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageTrackingCommandPrivate *d;
};
