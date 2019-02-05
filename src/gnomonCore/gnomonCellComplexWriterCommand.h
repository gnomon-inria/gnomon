#include "gnomonAbstractCommand.h"
#include "gnomonAbstractCellComplexWriter.h"

class GNOMONCORE_EXPORT gnomonCellComplexWriterCommand : public gnomonAbstractCommand
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
    void setCellComplex(gnomonCellComplex *cellcomplex);

private:
    class gnomonCellComplexWriterCommandPrivate *d;
};
