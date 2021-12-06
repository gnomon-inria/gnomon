#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonTreeWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonTreeWriterCommand(void);
    ~gnomonTreeWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setForm(gnomonAbstractDynamicForm *form) override;
    void setTree(gnomonTreeSeries *image_series);
    void setAlgorithmName(const QString& algo_name) override;

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonTreeWriterCommandPrivate *d;
};
