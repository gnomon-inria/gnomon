#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAdapterCommand.h"

class gnomonTree;

class GNOMONCORE_EXPORT gnomonTreeAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonTreeAdapterCommand(void);
    ~gnomonTreeAdapterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonTreeSeries *tree_series);
    void setAlgorithmName(const QString& algo_name) override;

public:
    gnomonTreeSeries *input(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonAbstractDynamicForm *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonTreeAdapterCommandPrivate *d;
};
