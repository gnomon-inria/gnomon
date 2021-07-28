#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonCellImageConstructorCommand(void) = delete;
     gnomonCellImageConstructorCommand(const QString&);
    ~gnomonCellImageConstructorCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    gnomonCellImageSeries *output(void);

    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, dtkCoreParameter *> parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageConstructorCommandPrivate *d;
};
