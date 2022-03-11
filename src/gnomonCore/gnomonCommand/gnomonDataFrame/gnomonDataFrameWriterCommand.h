#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonDataFrameWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonDataFrameWriterCommand();
    ~gnomonDataFrameWriterCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(gnomonAbstractDynamicForm *form) override;
    void setDataFrame(gnomonDataFrameSeries *image_series);
    void setAlgorithmName(const QString &) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

public:
    QStringList extensions(void) override;
    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "dataFrameWriter";
    static QStringList availablePlugins();

private:
    class gnomonDataFrameWriterCommandPrivate *d;
};
