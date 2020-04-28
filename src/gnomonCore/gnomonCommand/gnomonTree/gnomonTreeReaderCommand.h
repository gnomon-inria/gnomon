#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonTreeReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonTreeReaderCommand(void) = delete;
     gnomonTreeReaderCommand(const QString&);
    ~gnomonTreeReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonTreeSeries *tree(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonTreeReaderCommandPrivate *d;
};
