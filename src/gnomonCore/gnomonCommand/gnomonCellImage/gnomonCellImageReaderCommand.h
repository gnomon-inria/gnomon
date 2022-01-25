#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonCellImageReaderCommand();
    ~gnomonCellImageReaderCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    gnomonCellImageSeries *cellImage();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellImageReader";
    static QStringList availablePlugins();

private:
    class gnomonCellImageReaderCommandPrivate *d;
};
