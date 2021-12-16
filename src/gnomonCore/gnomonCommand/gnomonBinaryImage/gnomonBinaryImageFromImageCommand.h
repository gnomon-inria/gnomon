#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonBinaryImageFromImageCommand : public gnomonAbstractConstructorCommand
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
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonBinaryImageSeries *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);
    void setAlgorithmName(const QString &) override;

public:
    dtkCoreParameters parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonBinaryImageFromImageCommandPrivate *d;
};
