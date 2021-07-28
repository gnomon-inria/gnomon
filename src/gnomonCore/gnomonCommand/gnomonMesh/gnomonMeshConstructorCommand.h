#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonMeshConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonMeshConstructorCommand(void) = delete;
     gnomonMeshConstructorCommand(const QString&);
    ~gnomonMeshConstructorCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    gnomonMeshSeries *output(void);

    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, dtkCoreParameter *> parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonMeshConstructorCommandPrivate *d;
};
