#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonDataFrameWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonDataFrameWriterCommand(void);
    ~gnomonDataFrameWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    void setDataFrame(gnomonDataFrameSeries *image_series);
    void setAlgorithmName(const QString &) override;

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonDataFrameWriterCommandPrivate *d;
};
