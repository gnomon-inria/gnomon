#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class GNOMONCORE_EXPORT gnomonFemSolverCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonFemSolverCommand(void);
    ~gnomonFemSolverCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setMesh(std::shared_ptr<gnomonMeshSeries> mesh);
    std::shared_ptr<gnomonMeshSeries> inputMesh();
    void setAlgorithmName(const QString& algo_name) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void clear(void) override;

    std::shared_ptr<gnomonMeshSeries> updatedMesh();

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "femSolver";

private:
    class gnomonFemSolverCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonFemSolverCommand)