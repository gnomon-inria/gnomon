#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonDataDictWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonDataDictWriterCommand();
    ~gnomonDataDictWriterCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setForm(gnomonAbstractDynamicForm *) override;
    void setDataDict(gnomonDataDictSeries *dataDict);
    void setAlgorithmName(const QString &) override;

    orderedMap inputTypes() override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

public:
    static bool isEmpty();

private:
    class gnomonDataDictWriterCommandPrivate *d;
};