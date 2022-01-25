#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonImageReaderCommand: public gnomonAbstractReaderCommand
{
public:
     gnomonImageReaderCommand(void);
    ~gnomonImageReaderCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    gnomonImageSeries *image();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "imageReader";
    static QStringList availablePlugins();

private:
    class gnomonImageReaderCommandPrivate *d;
};
