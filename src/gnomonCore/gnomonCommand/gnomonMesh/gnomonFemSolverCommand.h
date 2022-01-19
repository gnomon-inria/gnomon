#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonFemSolverCommand : public gnomonAbstractCommand
{
public:
     gnomonFemSolverCommand();
    ~gnomonFemSolverCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setMesh(gnomonMeshSeries *mesh);
    gnomonMeshSeries* inputMesh();
    void setAlgorithmName(const QString& algo_name) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

    gnomonMeshSeries *updatedMesh();

public:
    static bool isEmpty();
    inline static const QString groupName = "femSolver";
    static QStringList availablePlugins();

private:
    class gnomonFemSolverCommandPrivate *d;
};
