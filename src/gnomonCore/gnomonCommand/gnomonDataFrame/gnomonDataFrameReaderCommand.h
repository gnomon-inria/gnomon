#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonDataFrameReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonDataFrameReaderCommand();
    ~gnomonDataFrameReaderCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonDataFrameSeries *dataFrame();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonDataFrameReaderCommandPrivate *d;
};
