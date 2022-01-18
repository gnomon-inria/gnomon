#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonDataDictReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonDataDictReaderCommand();
    ~gnomonDataDictReaderCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonDataDictSeries *dataDict();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonDataDictReaderCommandPrivate *d;
};
