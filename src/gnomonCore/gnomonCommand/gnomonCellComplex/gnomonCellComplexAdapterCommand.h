#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAdapterCommand.h"

class gnomonCellComplex;

class GNOMONCORE_EXPORT gnomonCellComplexAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonCellComplexAdapterCommand(void);
    ~gnomonCellComplexAdapterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonCellComplexSeries *cellComplex_series);
    void setAlgorithmName(const QString &) override;

    gnomonCellComplexSeries *input(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonAbstractDynamicForm *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellComplexAdapterCommandPrivate *d;
};
