#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonMeshWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonMeshWriterCommand(void);
    ~gnomonMeshWriterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(gnomonAbstractDynamicForm *form) override;
    void setMesh(gnomonMeshSeries *image_series);
    void setAlgorithmName(const QString& algo_name) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap inputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonMeshWriterCommandPrivate *d;
};
