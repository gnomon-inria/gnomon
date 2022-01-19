#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonCellImageConstructorCommand();
    ~gnomonCellImageConstructorCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonCellImageSeries *output();

    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellImageConstructor";
    static QStringList availablePlugins();

private:
    class gnomonCellImageConstructorCommandPrivate *d;
};
