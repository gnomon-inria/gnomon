#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAdapterCommand.h"

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonMeshAdapterCommand(void);
    ~gnomonMeshAdapterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonMeshSeries *mesh_series);
    void setAlgorithmName(const QString & algo) override;
    gnomonMeshSeries *input(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonAbstractDynamicForm *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonMeshAdapterCommandPrivate *d;
};
