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

public:
    QStringList extensions(void) override;
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