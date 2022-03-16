#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand.h>
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
    void setInput(gnomonCellComplexSeries *cellComplex_series);
    void setAlgorithmName(const QString &) override;

    gnomonCellComplexSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    gnomonAbstractDynamicForm *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap inputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellComplexAdapter";
    static QStringList availablePlugins();

private:
    class gnomonCellComplexAdapterCommandPrivate *d;
};
