#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>

#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>
#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class GNOMONCORE_EXPORT gnomonMeshFromImageCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonMeshFromImageCommand(void);
    ~gnomonMeshFromImageCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setImage(std::shared_ptr<gnomonImageSeries> image);
    std::shared_ptr<gnomonImageSeries> image();
    void setCellImage(std::shared_ptr<gnomonCellImageSeries> celllImage);
    std::shared_ptr<gnomonCellImageSeries> cellImage();
    void setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> binaryImage);
    std::shared_ptr<gnomonBinaryImageSeries> binaryImage();;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    std::shared_ptr<gnomonMeshSeries> output();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "meshFromImage";
    static QStringList availablePlugins();

private:
    class gnomonMeshFromImageCommandPrivate *d;
};
