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
    void setForm(gnomonAbstractDynamicForm *) override;
    void setDataDict(gnomonDataDictSeries *dataDict);
    void setAlgorithmName(const QString &) override;

public:
    orderedMap inputTypes() override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "dataDictWriter";
    static QStringList availablePlugins();

private:
    class gnomonDataDictWriterCommandPrivate *d;
};
