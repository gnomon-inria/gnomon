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
    std::shared_ptr<gnomonPointCloudSeries> pointCloud();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void clear(void) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "pointCloudReader";

private:
    class gnomonPointCloudReaderCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonPointCloudReaderCommand)