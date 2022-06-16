#pragma once

#include "gnomonCommand/gnomonAbstractReaderCommand.h"
#include <gnomonCore/gnomonForm/gnomonDataDict/gnomonDataDict>

class GNOMONCORE_EXPORT gnomonDataDictReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonDataDictReaderCommand();
    ~gnomonDataDictReaderCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonDataDictSeries> dataDict();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "dataDictReader";
    static QStringList availablePlugins();


private:
    class gnomonDataDictReaderCommandPrivate *d;
};
