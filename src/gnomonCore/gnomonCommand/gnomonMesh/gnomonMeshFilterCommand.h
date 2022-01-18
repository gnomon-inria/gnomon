#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshFilterCommand : public gnomonAbstractCommand
{
public:
     gnomonMeshFilterCommand(void);
    ~gnomonMeshFilterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonMeshSeries *mesh_series);
    gnomonMeshSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    gnomonMeshSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    static bool isEmpty();

private:
    class gnomonMeshFilterCommandPrivate *d;
};
