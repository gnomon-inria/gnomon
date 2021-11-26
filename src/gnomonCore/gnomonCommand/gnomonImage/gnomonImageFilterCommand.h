#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageFilterCommand : public gnomonAbstractCommand
{
public:
     gnomonImageFilterCommand(void);
    ~gnomonImageFilterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonImageSeries *image_series);
    gnomonImageSeries *input(void);

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;
    virtual orderedMap inputTypes(void) override;
    virtual void setInputForm(const QString& name, gnomonAbstractDynamicForm *form) override;

    gnomonImageSeries *output(void);

    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;
    virtual orderedMap outputTypes(void) override;

    virtual void setParameter(const QString&, const QVariant&);
    void setAlgorithmName(const QString &) override;

public:
    dtkCoreParameters parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonImageFilterCommandPrivate *d;
};
