#include "gnomonCommand/gnomonAbstractCommand.h"
class gnomonCellComplex;

class GNOMONCORE_EXPORT gnomonCellComplexReaderCommand : public gnomonAbstractCommand
{
public:
     gnomonCellComplexReaderCommand(void) = delete;
     gnomonCellComplexReaderCommand(const QString&);
    ~gnomonCellComplexReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonCellComplex *cellComplex(void);

private:
    class gnomonCellComplexReaderCommandPrivate *d;
};
