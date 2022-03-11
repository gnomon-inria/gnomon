#pragma once


#include <gnomonCore/gnomonCommand/gnomonAbstractAdapterCommand>

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>

class gnomonLString;

class GNOMONCORE_EXPORT gnomonLStringAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonLStringAdapterCommand(void);
    ~gnomonLStringAdapterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonLStringSeries *lString_series);
    void setAlgorithmName(const QString &) override;
    gnomonLStringSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap inputTypes() override;

    gnomonAbstractDynamicForm *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "lStringAdapter";
    static QStringList availablePlugins();

private:
    class gnomonLStringAdapterCommandPrivate *d;
};
