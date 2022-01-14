#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAdapterCommand.h"

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonMeshAdapterCommand();
    ~gnomonMeshAdapterCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setInput(gnomonMeshSeries *mesh_series);
    void setAlgorithmName(const QString & algo) override;
    gnomonMeshSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    gnomonAbstractDynamicForm *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "meshAdapter";
    static QStringList availablePlugins();

private:
    class gnomonMeshAdapterCommandPrivate *d;
};
