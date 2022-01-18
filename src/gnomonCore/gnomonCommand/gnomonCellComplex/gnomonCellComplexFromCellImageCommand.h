#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexFromCellImageCommand : public gnomonAbstractCommand
{
public:
     gnomonCellComplexFromCellImageCommand();
    ~gnomonCellComplexFromCellImageCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonCellImageSeries *image_series);

    gnomonCellImageSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

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
