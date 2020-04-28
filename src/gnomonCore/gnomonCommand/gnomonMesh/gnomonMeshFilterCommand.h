#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshFilterCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonMeshFilterCommand(void) = delete;
     gnomonMeshFilterCommand(const QString&);
    ~gnomonMeshFilterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonMeshSeries *mesh_series);
    gnomonMeshSeries *input(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonMeshSeries *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonMeshFilterCommandPrivate *d;
};
