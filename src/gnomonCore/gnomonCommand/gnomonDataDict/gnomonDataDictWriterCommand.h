#pragma once

#include "gnomonCommand/gnomonAbstractWriterCommand.h"
#include <gnomonCore/gnomonForm/gnomonDataDict/gnomonDataDict>

class GNOMONCORE_EXPORT gnomonDataDictWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonDataDictWriterCommand();
    ~gnomonDataDictWriterCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> ) override;
    void setDataDict(std::shared_ptr<gnomonDataDictSeries> dataDict);
    void setAlgorithmName(const QString &) override;

public:
    orderedMap inputTypes() override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "dataDictWriter";
    static QStringList availablePlugins();

private:
    class gnomonDataDictWriterCommandPrivate *d;
};
