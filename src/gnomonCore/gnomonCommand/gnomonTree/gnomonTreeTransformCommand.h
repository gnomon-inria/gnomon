#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

class gnomonTree;

class GNOMONCORE_EXPORT gnomonTreeTransformCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonTreeTransformCommand(void) = delete;
     gnomonTreeTransformCommand(const QString&);
    ~gnomonTreeTransformCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonTreeSeries *tree_series);
    gnomonTreeSeries *input(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonTreeSeries *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, dtkCoreParameter *> parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonTreeTransformCommandPrivate *d;
};
