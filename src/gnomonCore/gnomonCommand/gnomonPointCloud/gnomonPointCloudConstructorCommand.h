#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudConstructorCommand : public gnomonAbstractCommand
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

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonPointCloudConstructorCommandPrivate *d;
};
