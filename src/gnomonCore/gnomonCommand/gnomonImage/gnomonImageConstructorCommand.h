#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonImageConstructorCommand : public gnomonAbstractCommand
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

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonImageConstructorCommandPrivate *d;
};
