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
    gnomonTreeSeries *tree();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "treeReader";
    static QStringList availablePlugins();

private:
    class gnomonTreeReaderCommandPrivate *d;
};
