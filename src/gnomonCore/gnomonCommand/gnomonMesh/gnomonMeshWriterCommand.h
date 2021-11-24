#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonMeshWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonMeshWriterCommand(void);
    ~gnomonMeshWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setForm(gnomonAbstractDynamicForm *form) override;
    void setMesh(gnomonMeshSeries *image_series);
    void setAlgorithmName(const QString& algo_name) override;

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonMeshWriterCommandPrivate *d;
};
