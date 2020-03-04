#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshReaderCommand : public gnomonAbstractCommand
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

public:
    static bool isEmpty(void);

private:
    class gnomonMeshReaderCommandPrivate *d;
};
