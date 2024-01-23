#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>

class GNOMONCORE_EXPORT gnomonLStringReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonLStringReaderCommand(void);
    ~gnomonLStringReaderCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonLStringSeries> lString();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void clear(void) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "lStringReader";

private:
    class gnomonLStringReaderCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonLStringReaderCommand)