#pragma once

#include <gnomonVisualization/gnomonCommand/gnomonAbstractVtkVisualizationCommand>

#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud>

class GNOMONCORE_EXPORT gnomonPointCloudVtkVisualizationCommand : public gnomonAbstractVtkVisualizationCommand
{
public:
     gnomonPointCloudVtkVisualizationCommand();
    ~gnomonPointCloudVtkVisualizationCommand() override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void newVisualization(void) override;

    void setFormVisualization(const QString& visu_name, const QVariantMap &parameters) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;
    orderedMap inputTypes() override;
    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    void clear(void) override;

public:
    inline static const QString groupName = "pointCloudVtkVisualization";

private:
    class gnomonPointCloudVtkVisualizationCommandPrivate *d;
};
