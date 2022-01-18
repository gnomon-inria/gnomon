#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonCellComplexReaderCommand();
    ~gnomonCellComplexReaderCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    gnomonCellComplexSeries *cellComplex();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;
    orderedMap outputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonCellComplexReaderCommandPrivate *d;
};
