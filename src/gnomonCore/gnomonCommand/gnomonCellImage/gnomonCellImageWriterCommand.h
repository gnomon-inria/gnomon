#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonCellImageWriterCommand(void) = delete;
     gnomonCellImageWriterCommand(const QString&);
    ~gnomonCellImageWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    void setCellImage(gnomonCellImageSeries *image_series);

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageWriterCommandPrivate *d;
};
