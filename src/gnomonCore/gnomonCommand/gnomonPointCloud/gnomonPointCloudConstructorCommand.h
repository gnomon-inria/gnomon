#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud>

class GNOMONCORE_EXPORT gnomonPointCloudConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonPointCloudConstructorCommand(void);
    ~gnomonPointCloudConstructorCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonPointCloudSeries> output();

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    inline static const QString groupName = "pointCloudConstructor";

private:
    class gnomonPointCloudConstructorCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonPointCloudConstructorCommand)