#pragma once

#include <gnomonVisualization/gnomonCommand/gnomonAbstractVtkVisualizationCommand>

#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>

class GNOMONCORE_EXPORT gnomonImageVtkVisualizationCommand : public gnomonAbstractVtkVisualizationCommand
{
public:
     gnomonImageVtkVisualizationCommand();
    ~gnomonImageVtkVisualizationCommand() override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void newVisualization(void) override;

    void setFormVisualization(const QString& visu_name, const QVariantMap &parameters) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;
    orderedMap inputTypes() override;
    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    void clear(void) override;
    void clear(bool clear_visu) override;

public:
    inline static const QString groupName = "imageVtkVisualization";

private:
    class gnomonImageVtkVisualizationCommandPrivate *d;
};
