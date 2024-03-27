#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>

#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud>

class GNOMONCORE_EXPORT gnomonPointCloudFromImageCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonPointCloudFromImageCommand(void);
    ~gnomonPointCloudFromImageCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(std::shared_ptr<gnomonImageSeries> image);
    std::shared_ptr<gnomonImageSeries> input();
    void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage);
    std::shared_ptr<gnomonCellImageSeries> cellImage();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    std::shared_ptr<gnomonPointCloudSeries> output();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    void setAlgorithmName(const QString& algo_name) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "pointCloudFromImage";

private:
    class gnomonPointCloudFromImageCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonPointCloudFromImageCommand)