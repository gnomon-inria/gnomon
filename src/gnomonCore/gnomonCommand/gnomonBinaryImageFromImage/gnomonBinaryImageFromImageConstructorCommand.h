#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonBinaryImageFromImageConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonBinaryImageFromImageConstructorCommand(void);
    ~gnomonBinaryImageFromImageConstructorCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    gnomonBinaryImageSeries *output(void);

    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);
    void setAlgorithmName(const QString &) override;

public:
    dtkCoreParameters parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonBinaryImageFromImageConstructorCommandPrivate *d;
};
