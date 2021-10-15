#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonDataFrameReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonDataFrameReaderCommand(void);
    ~gnomonDataFrameReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonDataFrameSeries *dataFrame(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonDataFrameReaderCommandPrivate *d;
};
