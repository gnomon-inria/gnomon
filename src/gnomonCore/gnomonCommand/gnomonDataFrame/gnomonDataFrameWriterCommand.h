#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonDataFrameWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonDataFrameWriterCommand(void) = delete;
     gnomonDataFrameWriterCommand(const QString&);
    ~gnomonDataFrameWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    void setDataFrame(gnomonDataFrameSeries *image_series);

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonDataFrameWriterCommandPrivate *d;
};
