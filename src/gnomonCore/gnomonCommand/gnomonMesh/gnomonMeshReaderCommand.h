#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonMeshReaderCommand(void) = delete;
     gnomonMeshReaderCommand(const QString&);
    ~gnomonMeshReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonMeshSeries *mesh(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonMeshReaderCommandPrivate *d;
};
