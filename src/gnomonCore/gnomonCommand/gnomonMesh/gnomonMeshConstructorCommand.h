#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonMeshConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonMeshConstructorCommand();
    ~gnomonMeshConstructorCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonMeshSeries *output();

    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    static bool isEmpty();

private:
    class gnomonMeshConstructorCommandPrivate *d;
};
