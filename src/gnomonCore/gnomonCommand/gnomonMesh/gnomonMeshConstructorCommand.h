#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonMeshConstructorCommand : public gnomonAbstractCommand
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

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonMeshConstructorCommandPrivate *d;
};
