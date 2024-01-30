#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>

#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class GNOMONCORE_EXPORT gnomonTreeReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonTreeReaderCommand(void);
    ~gnomonTreeReaderCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonTreeSeries> tree();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "treeReader";

private:
    class gnomonTreeReaderCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonTreeReaderCommand)