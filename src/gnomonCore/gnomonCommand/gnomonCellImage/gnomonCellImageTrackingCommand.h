#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonForm/gnomonDataDict/gnomonDataDict>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>
#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class GNOMONCORE_EXPORT gnomonCellImageTrackingCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageTrackingCommand(void);
    ~gnomonCellImageTrackingCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setImage(std::shared_ptr<gnomonImageSeries> image);
    void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellimage);
    void setTransformation(std::shared_ptr<gnomonDataDictSeries> datadict);

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    std::shared_ptr<gnomonCellImageSeries> cellImage();
    std::shared_ptr<gnomonTreeSeries> tree();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellImageTracking";
    static QStringList availablePlugins();

private:
    class gnomonCellImageTrackingCommandPrivate *d;
};
