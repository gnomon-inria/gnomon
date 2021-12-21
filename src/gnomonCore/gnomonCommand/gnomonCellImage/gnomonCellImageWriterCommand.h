#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonCellImageWriterCommand();
    ~gnomonCellImageWriterCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setForm(gnomonAbstractDynamicForm *form) override;
    void setCellImage(gnomonCellImageSeries *image_series);
    void setAlgorithmName(const QString &) override;

    orderedMap inputTypes() override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

public:
    static bool isEmpty();

private:
    class gnomonCellImageWriterCommandPrivate *d;
};
