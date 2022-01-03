#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonMeshReaderCommand();
    ~gnomonMeshReaderCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonMeshSeries *mesh();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonMeshReaderCommandPrivate *d;
};
