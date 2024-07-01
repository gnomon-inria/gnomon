#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>

#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud>

class GNOMONCORE_EXPORT gnomonPointCloudWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonPointCloudWriterCommand(void);
    ~gnomonPointCloudWriterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void setPointCloud(std::shared_ptr<gnomonPointCloudSeries> pointCloud);
    void setAlgorithmName(const QString& algo_name) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

public:
    orderedMap inputTypes() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    void clear(void) override;

public:
    inline static const QString groupName = "pointCloudWriter";

private:
    class gnomonPointCloudWriterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonPointCloudWriterCommand)