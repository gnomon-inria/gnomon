#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

class GNOMONCORE_EXPORT gnomonTreeFromLStringCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonTreeFromLStringCommand(void) = delete;
     gnomonTreeFromLStringCommand(const QString&);
    ~gnomonTreeFromLStringCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonLStringSeries *lString_series);

    gnomonLStringSeries *input(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonTreeSeries *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, dtkCoreParameter *> parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonTreeFromLStringCommandPrivate *d;
};
