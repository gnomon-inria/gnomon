#include "gnomonAbstractCommand.h"
#include "gnomonAbstractMeshReader.h"

class GNOMONCORE_EXPORT gnomonMeshReaderCommand : public gnomonAbstractCommand<gnomonAbstractMeshReader>
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
    using gnomonAbstractCommand<gnomonAbstractMeshReader>::action;
};
