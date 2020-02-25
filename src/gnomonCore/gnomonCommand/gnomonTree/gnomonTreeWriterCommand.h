#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonTreeWriterCommand : public gnomonAbstractCommand
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
    void setInput(gnomonTreeSeries *tree_series);

public:
    static bool isEmpty(void);

private:
    class gnomonTreeWriterCommandPrivate *d;
};
