#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonTreeReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonTreeReaderCommand(void);
    ~gnomonTreeReaderCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    gnomonTreeSeries *tree();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "treeReader";
    static QStringList availablePlugins();

private:
    class gnomonTreeReaderCommandPrivate *d;
};
