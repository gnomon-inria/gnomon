#pragma once

#include <gnomonVisualization/gnomonCommand/gnomonAbstractMplVisualizationCommand>

#include <gnomonCore/gnomonForm/gnomonDataFrame/gnomonDataFrame>

class GNOMONCORE_EXPORT gnomonDataFrameMplVisualizationCommand : public gnomonAbstractMplVisualizationCommand
{
public:
     gnomonDataFrameMplVisualizationCommand();
    ~gnomonDataFrameMplVisualizationCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void setAlgorithmName(const QString &) override;

    void setFormVisualization(const QString& visu_name, const QVariantMap &parameters) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;
    orderedMap inputTypes() override;
    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "dataFrameMplVisualization";
    static QStringList availablePlugins();

private:
    class gnomonDataFrameMplVisualizationCommandPrivate *d;
};
