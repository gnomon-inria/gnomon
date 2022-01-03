#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonTreeReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonTreeReaderCommand();
    ~gnomonTreeReaderCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonTreeSeries *tree();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonTreeReaderCommandPrivate *d;
};
