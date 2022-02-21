#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractConstructorCommand.h"

class GNOMONCORE_EXPORT gnomonCellComplexConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonCellComplexConstructorCommand();
    ~gnomonCellComplexConstructorCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    gnomonCellComplexSeries *output();

    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();;
    inline static const QString groupName = "cellComplexConstructor";
    static QStringList availablePlugins();

private:
    class gnomonCellComplexConstructorCommandPrivate *d;
};
