#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonImageWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonImageWriterCommand(void) = delete;
     gnomonImageWriterCommand(const QString&);
    ~gnomonImageWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    void setImage(gnomonImageSeries *image_series);

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonImageWriterCommandPrivate *d;
};
