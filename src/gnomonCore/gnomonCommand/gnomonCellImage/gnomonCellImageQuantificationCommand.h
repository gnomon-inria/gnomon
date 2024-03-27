#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>
#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame>

class GNOMONCORE_EXPORT gnomonCellImageQuantificationCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonCellImageQuantificationCommand();
    ~gnomonCellImageQuantificationCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setImage(std::shared_ptr<gnomonImageSeries> image);
    void setMesh(std::shared_ptr<gnomonMeshSeries> mesh);
    void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellimage);

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    std::shared_ptr<gnomonCellImageSeries> cellImage();
    std::shared_ptr<gnomonDataFrameSeries> dataFrame();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString &) override;

public:
    inline static const QString groupName = "cellImageQuantification";

private:
    class gnomonCellImageQuantificationCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonCellImageQuantificationCommand)