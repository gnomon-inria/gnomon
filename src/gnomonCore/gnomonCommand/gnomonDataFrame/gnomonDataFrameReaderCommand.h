#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonDataFrameReaderCommand : public gnomonAbstractCommand
{
public:
     gnomonDataFrameReaderCommand(void) = delete;
     gnomonDataFrameReaderCommand(const QString&);
    ~gnomonDataFrameReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonDataFrameSeries *dataFrame(void);

public:
    static bool isEmpty(void);

private:
    class gnomonDataFrameReaderCommandPrivate *d;
};
