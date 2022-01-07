#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonTreeFromLStringCommand : public gnomonAbstractCommand
{
public:
     gnomonTreeFromLStringCommand();
    ~gnomonTreeFromLStringCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setInput(gnomonLStringSeries *lString_series);

    gnomonLStringSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    gnomonTreeSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();

private:
    class gnomonTreeFromLStringCommandPrivate *d;
};
