#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonCellComplexWriterCommand(void) = delete;
     gnomonCellComplexWriterCommand(const QString&);
    ~gnomonCellComplexWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    void setCellComplex(gnomonCellComplexSeries *cellcomplex);

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellComplexWriterCommandPrivate *d;
};
