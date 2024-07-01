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
    void setInput(std::shared_ptr<gnomonLStringSeries> lString_series);
    void setAlgorithmName(const QString &) override;
    std::shared_ptr<gnomonLStringSeries> input();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    orderedMap inputTypes() override;

    std::shared_ptr<gnomonAbstractDynamicForm> output();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    void clear(void) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "lStringAdapter";

private:
    class gnomonLStringAdapterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonLStringAdapterCommand)