#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonImageConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonImageConstructorCommand();
    ~gnomonImageConstructorCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonImageSeries *output();

    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "imageConstructor";
    static QStringList availablePlugins();

private:
    class gnomonImageConstructorCommandPrivate *d;
};
