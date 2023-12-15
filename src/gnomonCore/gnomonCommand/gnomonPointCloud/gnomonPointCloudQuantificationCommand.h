#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>

#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud>

class GNOMONCORE_EXPORT gnomonPointCloudQuantificationCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonPointCloudQuantificationCommand(void);
    ~gnomonPointCloudQuantificationCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setImage(std::shared_ptr<gnomonImageSeries> image);
    void setMesh(std::shared_ptr<gnomonMeshSeries> mesh);
    void setPointCloud(std::shared_ptr<gnomonPointCloudSeries> pointCloud);
    void setAlgorithmName(const QString &) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    std::shared_ptr<gnomonPointCloudSeries> pointCloud();
    std::shared_ptr<gnomonDataFrameSeries> dataFrame();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "pointCloudQuantification";

private:
    class gnomonPointCloudQuantificationCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonPointCloudQuantificationCommand)