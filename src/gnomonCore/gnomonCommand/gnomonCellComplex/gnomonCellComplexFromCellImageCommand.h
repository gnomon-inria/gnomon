#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexFromCellImageCommand : public gnomonAbstractCommand
{
public:
     gnomonCellComplexFromCellImageCommand();
    ~gnomonCellComplexFromCellImageCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setInput(gnomonCellImageSeries *image_series);

    gnomonCellImageSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    gnomonCellComplexSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;


    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();

private:
    class gnomonCellComplexFromCellImageCommandPrivate *d;
};
