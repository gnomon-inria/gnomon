#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAdapterCommand.h"

class gnomonLString;

class GNOMONCORE_EXPORT gnomonLStringAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonLStringAdapterCommand();
    ~gnomonLStringAdapterCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setInput(gnomonLStringSeries *lString_series);
    void setAlgorithmName(const QString &) override;
    gnomonLStringSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;

    gnomonAbstractDynamicForm *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

public:
    static bool isEmpty();

private:
    class gnomonLStringAdapterCommandPrivate *d;
};
