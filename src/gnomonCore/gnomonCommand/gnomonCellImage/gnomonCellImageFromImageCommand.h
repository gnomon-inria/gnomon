#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>

#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud>

class GNOMONCORE_EXPORT gnomonCellImageFromImageCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonCellImageFromImageCommand();
    ~gnomonCellImageFromImageCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(std::shared_ptr<gnomonImageSeries> image_series); 
    std::shared_ptr<gnomonImageSeries> input();

public:
    void setCellPoints(std::shared_ptr<gnomonPointCloudSeries> pointCloud_series);
    std::shared_ptr<gnomonPointCloudSeries> cellPoints();
    void setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> binary_image_series);
    std::shared_ptr<gnomonBinaryImageSeries> binaryImage();;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;
    orderedMap inputTypes() override;
    void setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

public:
    void setAlgorithmName(const QString &) override;

public:
    std::shared_ptr<gnomonCellImageSeries> output();

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;
    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellImageFromImage";
    static QStringList availablePlugins();

private:
    class gnomonCellImageFromImageCommandPrivate *d;
};

//
// gnomonCellImageFromImageCommand.h ends here
