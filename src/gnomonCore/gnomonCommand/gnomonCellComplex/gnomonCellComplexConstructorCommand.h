#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexConstructorCommand : public gnomonAbstractCommand
{
public:
     gnomonCellComplexConstructorCommand(void) = delete;
     gnomonCellComplexConstructorCommand(const QString&);
    ~gnomonCellComplexConstructorCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    gnomonCellComplexSeries *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonCellComplexConstructorCommandPrivate *d;
};
