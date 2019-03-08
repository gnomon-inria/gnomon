#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonFemSolverCommand : public gnomonAbstractCommand
{
public:
     gnomonFemSolverCommand(void) = delete;
     gnomonFemSolverCommand(const QString&);
    ~gnomonFemSolverCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setMesh(gnomonMesh *mesh);
    void setParameter(const QString&, const QVariant&);

    QMap<QString, gnomonCoreParameter *> parameters(void) const;
    gnomonMesh *updatedMesh(void);

private:
    class gnomonFemSolverCommandPrivate *d;
};
