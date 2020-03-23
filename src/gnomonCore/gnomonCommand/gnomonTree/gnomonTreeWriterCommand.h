#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonTreeWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonTreeWriterCommand(void) = delete;
     gnomonTreeWriterCommand(const QString&);
    ~gnomonTreeWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    void setTree(gnomonTreeSeries *image_series);

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonTreeWriterCommandPrivate *d;
};
