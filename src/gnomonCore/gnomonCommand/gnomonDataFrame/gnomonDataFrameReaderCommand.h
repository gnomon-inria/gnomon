#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonDataFrameReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonDataFrameReaderCommand();
    ~gnomonDataFrameReaderCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    gnomonDataFrameSeries *dataFrame();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "dataFrameReader";
    static QStringList availablePlugins();

private:
    class gnomonDataFrameReaderCommandPrivate *d;
};
