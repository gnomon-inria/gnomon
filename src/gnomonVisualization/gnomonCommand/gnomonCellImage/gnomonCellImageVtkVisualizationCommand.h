#pragma once

#include <gnomonVisualization/gnomonCommand/gnomonAbstractVtkVisualizationCommand>

#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>

class GNOMONCORE_EXPORT gnomonCellImageVtkVisualizationCommand : public gnomonAbstractVtkVisualizationCommand
{
public:
     gnomonCellImageVtkVisualizationCommand();
    ~gnomonCellImageVtkVisualizationCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void setAlgorithmName(const QString &) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;
    orderedMap inputTypes() override;
    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "visualizationCellImage";
    static QStringList availablePlugins();

private:
    class gnomonCellImageVtkVisualizationCommandPrivate *d;
};
