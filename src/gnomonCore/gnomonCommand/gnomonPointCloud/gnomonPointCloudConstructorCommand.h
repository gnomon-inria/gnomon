#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonPointCloudConstructorCommand();
    ~gnomonPointCloudConstructorCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonPointCloudSeries *output();

    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "pointCloudConstructor";
    static QStringList availablePlugins();

private:
    class gnomonPointCloudConstructorCommandPrivate *d;
};
