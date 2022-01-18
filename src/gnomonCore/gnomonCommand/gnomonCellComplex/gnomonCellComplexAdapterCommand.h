#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAdapterCommand.h"

class gnomonCellComplex;

class GNOMONCORE_EXPORT gnomonCellComplexAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonCellComplexAdapterCommand();
    ~gnomonCellComplexAdapterCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonCellComplexSeries *cellComplex_series);
    void setAlgorithmName(const QString &) override;

    gnomonCellComplexSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    gnomonAbstractDynamicForm *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap inputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonCellComplexAdapterCommandPrivate *d;
};
