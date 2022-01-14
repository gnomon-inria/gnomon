#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class gnomonPointCloud;

class GNOMONCORE_EXPORT gnomonPointCloudReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonPointCloudReaderCommand();
    ~gnomonPointCloudReaderCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonPointCloudSeries *pointCloud();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "pointCloudReader";
    static QStringList availablePlugins();

private:
    class gnomonPointCloudReaderCommandPrivate *d;
};
