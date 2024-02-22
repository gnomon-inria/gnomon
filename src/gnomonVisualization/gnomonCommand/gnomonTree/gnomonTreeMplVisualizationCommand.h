#pragma once

#include <gnomonVisualization/gnomonCommand/gnomonAbstractMplVisualizationCommand>

#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class GNOMONCORE_EXPORT gnomonTreeMplVisualizationCommand : public gnomonAbstractMplVisualizationCommand
{
public:
     gnomonTreeMplVisualizationCommand();
    ~gnomonTreeMplVisualizationCommand() override;

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

    void clear(void) override;
    void clear(bool clear_visu) override;

public:
    inline static const QString groupName = "treeMplVisualization";

private:
    class gnomonTreeMplVisualizationCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonTreeMplVisualizationCommand)