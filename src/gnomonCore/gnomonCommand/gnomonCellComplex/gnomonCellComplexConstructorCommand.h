#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonCellComplexConstructorCommand(void) = delete;
     gnomonCellComplexConstructorCommand(const QString&);
    ~gnomonCellComplexConstructorCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    gnomonCellComplexSeries *output(void);

    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    dtkCoreParameters parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellComplexConstructorCommandPrivate *d;
};
