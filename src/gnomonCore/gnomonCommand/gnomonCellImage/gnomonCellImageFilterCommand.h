#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonCellImage;

class GNOMONCORE_EXPORT gnomonCellImageFilterCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageFilterCommand();
    ~gnomonCellImageFilterCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setInput(gnomonCellImageSeries *cellImage_series);
    gnomonCellImageSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    gnomonCellImageSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();

private:
    class gnomonCellImageFilterCommandPrivate *d;
};
