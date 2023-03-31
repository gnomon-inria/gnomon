#pragma once

#include <gnomonVisualization/gnomonCommand/gnomonAbstractVtkVisualizationCommand>

#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage>

class GNOMONCORE_EXPORT gnomonBinaryImageVtkVisualizationCommand : public gnomonAbstractVtkVisualizationCommand
{
public:
     gnomonBinaryImageVtkVisualizationCommand();
    ~gnomonBinaryImageVtkVisualizationCommand() override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void setAlgorithmName(const QString &) override;

    void setFormVisualization(const QString& visu_name, const QVariantMap &parameters) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;
    orderedMap inputTypes() override;
    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

public:
    inline static const QString groupName = "binaryImageVtkVisualization";

private:
    class gnomonBinaryImageVtkVisualizationCommandPrivate *d;
};
