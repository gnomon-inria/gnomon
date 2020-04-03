#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

class gnomonCellImage;

class GNOMONCORE_EXPORT gnomonCellImageFilterCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonCellImageFilterCommand(void) = delete;
     gnomonCellImageFilterCommand(const QString&);
    ~gnomonCellImageFilterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonCellImageSeries *cellImage_series);
    gnomonCellImageSeries *input(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonCellImageSeries *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageFilterCommandPrivate *d;
};
