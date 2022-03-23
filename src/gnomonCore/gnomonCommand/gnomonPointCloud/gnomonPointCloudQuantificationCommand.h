#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>

#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud>

class GNOMONCORE_EXPORT gnomonPointCloudQuantificationCommand : public gnomonAbstractCommand
{
public:
     gnomonPointCloudQuantificationCommand(void);
    ~gnomonPointCloudQuantificationCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setImage(gnomonImageSeries *image);
    void setPointCloud(gnomonPointCloudSeries *pointCloud);
    void setAlgorithmName(const QString &) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    gnomonPointCloudSeries *pointCloud();
    gnomonDataFrameSeries *dataFrame();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "pointCloudQuantification";
    static QStringList availablePlugins();

private:
    class gnomonPointCloudQuantificationCommandPrivate *d;
};
