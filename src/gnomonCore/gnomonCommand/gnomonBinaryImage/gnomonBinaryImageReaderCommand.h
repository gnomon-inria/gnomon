#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonBinaryImageReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonBinaryImageReaderCommand();
    ~gnomonBinaryImageReaderCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonBinaryImageSeries *binaryImage();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "binaryImageReader";
    static QStringList availablePlugins();

private:
    class gnomonBinaryImageReaderCommandPrivate *d;
};
