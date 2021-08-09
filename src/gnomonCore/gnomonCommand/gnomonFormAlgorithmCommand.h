#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"


class GNOMONCORE_EXPORT gnomonFormAlgorithmCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonFormAlgorithmCommand(void) = delete;
     gnomonFormAlgorithmCommand(const QString&);
    ~gnomonFormAlgorithmCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void addInput(gnomonAbstractDynamicForm *input);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    void addOutput(gnomonAbstractDynamicForm *output);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    void addParameter(const QString&, dtkCoreParameter *parameter);

public:
    dtkCoreParameters parameters(void) const override;

private:
    class gnomonFormAlgorithmCommandPrivate *d;
};
