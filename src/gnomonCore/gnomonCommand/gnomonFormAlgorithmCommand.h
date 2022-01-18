#include "gnomonCommand/gnomonAbstractCommand.h"

// TODO reemove ? is it used somewhere?
// NOTE: Good question !
class GNOMONCORE_EXPORT gnomonFormAlgorithmCommand : public gnomonAbstractCommand
{
public:
     gnomonFormAlgorithmCommand(void) = delete;
     gnomonFormAlgorithmCommand(const QString&);
    ~gnomonFormAlgorithmCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void addInput(gnomonAbstractDynamicForm *input);
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void addOutput(gnomonAbstractDynamicForm *output);
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void addParameter(const QString&, dtkCoreParameter *parameter);

public:
    dtkCoreParameters parameters() const override;

private:
    class gnomonFormAlgorithmCommandPrivate *d;
};
