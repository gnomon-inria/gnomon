#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand.h>
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>

class GNOMONCORE_EXPORT gnomonCellComplexFromCellImageCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonCellComplexFromCellImageCommand();
    ~gnomonCellComplexFromCellImageCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(std::shared_ptr<gnomonCellImageSeries> image_series);

    std::shared_ptr<gnomonCellImageSeries> input();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    std::shared_ptr<gnomonCellComplexSeries> output();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellComplexFromCellImage";
    static QStringList availablePlugins();

private:
    class gnomonCellComplexFromCellImageCommandPrivate *d;
};
