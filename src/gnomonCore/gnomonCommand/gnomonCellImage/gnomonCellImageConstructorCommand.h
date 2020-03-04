#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageConstructorCommand : public gnomonAbstractCommand
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

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageConstructorCommandPrivate *d;
};
