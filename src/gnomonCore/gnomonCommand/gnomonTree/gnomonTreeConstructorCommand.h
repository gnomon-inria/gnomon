#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonTreeConstructorCommand : public gnomonAbstractCommand
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

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonTreeConstructorCommandPrivate *d;
};
