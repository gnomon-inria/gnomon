#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonCellComplexConstructorCommand(void);
    ~gnomonCellComplexConstructorCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    gnomonCellComplexSeries *output(void);

    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);
    void setAlgorithmName(const QString &) override;

public:
    dtkCoreParameters parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellComplexConstructorCommandPrivate *d;
};
