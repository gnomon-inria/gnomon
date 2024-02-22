#pragma once

#include <gnomonVisualization/gnomonCommand/gnomonAbstractVtkVisualizationCommand>

#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex>

class GNOMONCORE_EXPORT gnomonCellComplexVtkVisualizationCommand : public gnomonAbstractVtkVisualizationCommand
{
public:
     gnomonCellComplexVtkVisualizationCommand();
    ~gnomonCellComplexVtkVisualizationCommand() override;

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
    inline static const QString groupName = "cellComplexVtkVisualization";

private:
    class gnomonCellComplexVtkVisualizationCommandPrivate *d;
};
