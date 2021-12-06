#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonCellComplexWriterCommand(void);
    ~gnomonCellComplexWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setForm(gnomonAbstractDynamicForm *) override;
    void setCellComplex(gnomonCellComplexSeries *cellcomplex);
    void setAlgorithmName(const QString &) override;

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellComplexWriterCommandPrivate *d;
};
