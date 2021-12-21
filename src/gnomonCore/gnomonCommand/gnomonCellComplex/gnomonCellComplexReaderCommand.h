#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonCellComplexReaderCommand();
    ~gnomonCellComplexReaderCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonCellComplexSeries *cellComplex();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;
    orderedMap outputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonCellComplexReaderCommandPrivate *d;
};
