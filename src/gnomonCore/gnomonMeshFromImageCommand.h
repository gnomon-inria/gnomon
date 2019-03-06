#include "gnomonAbstractCommand.h"

class gnomonImagesSerie;

class GNOMONCORE_EXPORT gnomonMeshFromImageCommand : public gnomonAbstractCommand
{
public:
     gnomonMeshFromImageCommand(void) = delete;
     gnomonMeshFromImageCommand(const QString&);
    ~gnomonMeshFromImageCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonImagesSerie *images_serie);
    gnomonImagesSerie *input(void);

    gnomonMesh *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonMeshFromImageCommandPrivate *d;
};
