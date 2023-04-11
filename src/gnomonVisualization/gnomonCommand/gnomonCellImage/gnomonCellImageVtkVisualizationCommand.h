#pragma once

#include <gnomonVisualization/gnomonCommand/gnomonAbstractVtkVisualizationCommand>

#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>

class GNOMONCORE_EXPORT gnomonCellImageVtkVisualizationCommand : public gnomonAbstractVtkVisualizationCommand
{
public:
     gnomonCellImageVtkVisualizationCommand();
    ~gnomonCellImageVtkVisualizationCommand() override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void newVisualization(void) override;

    void setFormVisualization(const QString& visu_name, const QVariantMap &parameters) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;
    orderedMap inputTypes() override;
    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

public:
    inline static const QString groupName = "cellImageVtkVisualization";

private:
    class gnomonCellImageVtkVisualizationCommandPrivate *d;
};
