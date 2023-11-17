#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand.h>
#include <gnomonCore/gnomonCommand/gnomonAbstractAdapterCommand.h>
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex.h>

class gnomonCellComplex;

class GNOMONCORE_EXPORT gnomonCellComplexAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonCellComplexAdapterCommand();
    ~gnomonCellComplexAdapterCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(std::shared_ptr<gnomonCellComplexSeries> cellComplex_series);
    void setAlgorithmName(const QString &) override;

    std::shared_ptr<gnomonCellComplexSeries> input();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    std::shared_ptr<gnomonAbstractDynamicForm> output();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    orderedMap inputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellComplexAdapter";
    static QStringList availablePlugins();

private:
    class gnomonCellComplexAdapterCommandPrivate *d;
};
