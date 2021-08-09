#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonImageConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonImageConstructorCommand(void) = delete;
     gnomonImageConstructorCommand(const QString&);
    ~gnomonImageConstructorCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    gnomonImageSeries *output(void);

    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    dtkCoreParameters parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonImageConstructorCommandPrivate *d;
};
