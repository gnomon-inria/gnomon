#include "gnomonCommand/gnomonAbstractCommand.h"

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

    gnomonCellComplexSeries *cellComplex(void);

public:
    static bool isEmpty(void);

private:
    class gnomonCellComplexReaderCommandPrivate *d;
};
