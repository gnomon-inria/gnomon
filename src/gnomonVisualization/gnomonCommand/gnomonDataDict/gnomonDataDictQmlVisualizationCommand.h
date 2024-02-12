#pragma once

#include <gnomonVisualization/gnomonCommand/gnomonAbstractQmlVisualizationCommand>

#include <gnomonCore/gnomonForm/gnomonDataDict/gnomonDataDict>

class GNOMONCORE_EXPORT gnomonDataDictQmlVisualizationCommand : public gnomonAbstractQmlVisualizationCommand
{
public:
     gnomonDataDictQmlVisualizationCommand();
    ~gnomonDataDictQmlVisualizationCommand() override;

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
    inline static const QString groupName = "dataDictQmlVisualization";

private:
    class gnomonDataDictQmlVisualizationCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonDataDictQmlVisualizationCommand)