#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonPointCloudConstructorCommand(void) = delete;
     gnomonPointCloudConstructorCommand(const QString&);
    ~gnomonPointCloudConstructorCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    gnomonPointCloudSeries *output(void);

    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonPointCloudConstructorCommandPrivate *d;
};
