#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAdapterCommand.h"

class gnomonTree;

class GNOMONCORE_EXPORT gnomonTreeAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonTreeAdapterCommand(void);
    ~gnomonTreeAdapterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonTreeSeries *tree_series);
    void setAlgorithmName(const QString& algo_name) override;

public:
    gnomonTreeSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    gnomonAbstractDynamicForm *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonTreeAdapterCommandPrivate *d;
};
