#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonImageWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonImageWriterCommand(void);
    ~gnomonImageWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setForm(gnomonAbstractDynamicForm *form) override;
    void setImage(gnomonImageSeries *image_series);
    void setAlgorithmName(const QString &) override;

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonImageWriterCommandPrivate *d;
};
