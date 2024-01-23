#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>

class GNOMONCORE_EXPORT gnomonMeshFilterCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonMeshFilterCommand(void);
    ~gnomonMeshFilterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(std::shared_ptr<gnomonMeshSeries> mesh_series);
    std::shared_ptr<gnomonMeshSeries> input();
    void setCellImage(std::shared_ptr<gnomonCellImageSeries> celllImage);
    std::shared_ptr<gnomonCellImageSeries> cellImage();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    std::shared_ptr<gnomonMeshSeries> output();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    void clear(void) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    inline static const QString groupName = "meshFilter";

private:
    class gnomonMeshFilterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonMeshFilterCommand)