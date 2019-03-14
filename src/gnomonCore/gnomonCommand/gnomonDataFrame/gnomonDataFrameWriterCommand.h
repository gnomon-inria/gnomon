#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImagesSerie;

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
    void setDataFrame(gnomonDataFrame *dataFrame);

private:
    class gnomonDataFrameWriterCommandPrivate *d;
};
