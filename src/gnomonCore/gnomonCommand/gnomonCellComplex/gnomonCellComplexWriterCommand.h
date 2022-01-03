#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonCellComplexWriterCommand();
    ~gnomonCellComplexWriterCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setForm(gnomonAbstractDynamicForm *) override;
    void setCellComplex(gnomonCellComplexSeries *cellcomplex);
    void setAlgorithmName(const QString &) override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

public:
    static bool isEmpty();

private:
    class gnomonCellComplexWriterCommandPrivate *d;
};
