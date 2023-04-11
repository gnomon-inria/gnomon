#pragma once

#include <gnomonVisualization/gnomonCommand/gnomonAbstractMplVisualizationCommand>

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>

class GNOMONCORE_EXPORT gnomonLStringMplVisualizationCommand : public gnomonAbstractMplVisualizationCommand
{
public:
     gnomonLStringMplVisualizationCommand();
    ~gnomonLStringMplVisualizationCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void newVisualization(void) override;

    void setFormVisualization(const QString& visu_name, const QVariantMap &parameters) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;
    orderedMap inputTypes() override;
    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "lStringMplVisualization";
    static QStringList availablePlugins();

private:
    class gnomonLStringMplVisualizationCommandPrivate *d;
};
