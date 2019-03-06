#include "gnomonAbstractCommand.h"

class gnomonImagesSerie;

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

    gnomonMesh *mesh(void);

private:
    class gnomonMeshReaderCommandPrivate *d;
};
