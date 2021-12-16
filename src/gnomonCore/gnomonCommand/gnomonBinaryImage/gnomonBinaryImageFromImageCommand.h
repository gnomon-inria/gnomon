#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"


class GNOMONCORE_EXPORT gnomonBinaryImageFromImageCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonBinaryImageFromImageCommand(void);
    ~gnomonBinaryImageFromImageCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonImageSeries *image);
    gnomonImageSeries *input(void);
    void setInitialization(gnomonBinaryImageSeries *init);
    gnomonBinaryImageSeries *initialization(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    virtual orderedMap inputTypes(void) override;
    virtual void setInputForm(const QString& name, gnomonAbstractDynamicForm *form) override;

    gnomonBinaryImageSeries *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;
    virtual orderedMap outputTypes(void);

    virtual void setParameter(const QString&, const QVariant&);
    void setAlgorithmName(const QString &) override;

public:
    dtkCoreParameters parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonBinaryImageFromImageCommandPrivate *d;
};
