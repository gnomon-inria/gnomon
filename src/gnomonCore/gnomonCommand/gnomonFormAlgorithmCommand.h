#include "gnomonCommand/gnomonAbstractCommand.h"

//TODO reemove ? is it used somewhere?
class GNOMONCORE_EXPORT gnomonFormAlgorithmCommand : public gnomonAbstractCommand
{
public:
     gnomonFormAlgorithmCommand() = delete;
     gnomonFormAlgorithmCommand(const QString&);
    ~gnomonFormAlgorithmCommand() override;

public:
    void redo() override;
    void undo() override;

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
