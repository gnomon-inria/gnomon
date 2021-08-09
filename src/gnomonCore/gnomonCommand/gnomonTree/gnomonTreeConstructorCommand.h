#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonTreeConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonTreeConstructorCommand(void) = delete;
     gnomonTreeConstructorCommand(const QString&);
    ~gnomonTreeConstructorCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    gnomonTreeSeries *output(void);

    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    dtkCoreParameters parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonTreeConstructorCommandPrivate *d;
};
