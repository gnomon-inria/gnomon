#include "gnomonAbstractCommand.h"

class gnomonImagesSerie;

class GNOMONCORE_EXPORT gnomonMeshWriterCommand : public gnomonAbstractCommand
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
    void setMesh(gnomonMesh *images_serie);

private:
    class gnomonMeshWriterCommandPrivate *d;
};
