#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonDataFrameWriterCommand : public gnomonAbstractCommand
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
    void setDataFrame(gnomonDataFrameSeries *dataFrame);

public:
    static bool isEmpty(void);

private:
    class gnomonDataFrameWriterCommandPrivate *d;
};
