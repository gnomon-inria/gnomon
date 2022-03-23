#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>

#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud>

class gnomonPointCloud;

class GNOMONCORE_EXPORT gnomonPointCloudReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonPointCloudReaderCommand(void);
    ~gnomonPointCloudReaderCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    gnomonPointCloudSeries *pointCloud();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "pointCloudReader";
    static QStringList availablePlugins();

private:
    class gnomonPointCloudReaderCommandPrivate *d;
};
