#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonMeshWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonMeshWriterCommand(void) = delete;
     gnomonMeshWriterCommand(const QString&);
    ~gnomonMeshWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    void setMesh(gnomonMeshSeries *image_series);

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonMeshWriterCommandPrivate *d;
};
