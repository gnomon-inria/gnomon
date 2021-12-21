#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonTreeConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonTreeConstructorCommand();
    ~gnomonTreeConstructorCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonTreeSeries *output();

    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    static bool isEmpty();

private:
    class gnomonTreeConstructorCommandPrivate *d;
};
