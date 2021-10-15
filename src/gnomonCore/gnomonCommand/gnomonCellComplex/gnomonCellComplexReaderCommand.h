#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonCellComplexReaderCommand(void);
    ~gnomonCellComplexReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonCellComplexSeries *cellComplex(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonCellComplexReaderCommandPrivate *d;
};
