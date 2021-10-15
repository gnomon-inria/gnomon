#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAdapterCommand.h"

class gnomonLString;

class GNOMONCORE_EXPORT gnomonLStringAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonLStringAdapterCommand(void);
    ~gnomonLStringAdapterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonLStringSeries *lString_series);
    void setAlgorithmName(const QString &) override;
    gnomonLStringSeries *input(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonAbstractDynamicForm *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonLStringAdapterCommandPrivate *d;
};
