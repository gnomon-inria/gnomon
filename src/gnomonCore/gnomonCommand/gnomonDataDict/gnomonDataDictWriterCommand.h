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

    void clear(void) override;

public:
    inline static const QString groupName = "dataDictWriter";

private:
    class gnomonDataDictWriterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonDataDictWriterCommand)