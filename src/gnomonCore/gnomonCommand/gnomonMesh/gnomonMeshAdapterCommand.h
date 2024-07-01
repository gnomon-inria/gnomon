#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAdapterCommand>

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonMeshAdapterCommand(void);
    ~gnomonMeshAdapterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(std::shared_ptr<gnomonMeshSeries> mesh_series);
    void setAlgorithmName(const QString & algo) override;
    std::shared_ptr<gnomonMeshSeries> input();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    std::shared_ptr<gnomonAbstractDynamicForm> output();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    void clear(void) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "meshAdapter";

private:
    class gnomonMeshAdapterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonMeshAdapterCommand)