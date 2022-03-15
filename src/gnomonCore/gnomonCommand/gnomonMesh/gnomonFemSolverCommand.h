#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class GNOMONCORE_EXPORT gnomonFemSolverCommand : public gnomonAbstractCommand
{
public:
     gnomonFemSolverCommand(void);
    ~gnomonFemSolverCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

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

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "femSolver";
    static QStringList availablePlugins();

private:
    class gnomonFemSolverCommandPrivate *d;
};
