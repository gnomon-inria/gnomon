#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>
#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class GNOMONCORE_EXPORT gnomonTreeFromLStringCommand : public gnomonAbstractCommand
{
public:
     gnomonTreeFromLStringCommand(void);
    ~gnomonTreeFromLStringCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonLStringSeries *lString_series);

    gnomonLStringSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    gnomonTreeSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "treeFromLString";
    static QStringList availablePlugins();

private:
    class gnomonTreeFromLStringCommandPrivate *d;
};
