#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonTree;

class GNOMONCORE_EXPORT gnomonTreeTransformCommand : public gnomonAbstractCommand
{
public:
     gnomonTreeTransformCommand(void);
    ~gnomonTreeTransformCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonTreeSeries *tree_series);
    gnomonTreeSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    gnomonTreeSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    static bool isEmpty();

private:
    class gnomonTreeTransformCommandPrivate *d;
};
