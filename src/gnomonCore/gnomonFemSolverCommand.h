#include "gnomonAbstractCommand.h"
#include "gnomonAbstractFemSolver.h"

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonFemSolverCommand : public gnomonAbstractCommand<gnomonAbstractFemSolver>
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
    
    virtual QMap<QString, gnomonParameter*> parameters(void) const;

    virtual void setParameter(const QString&, const QVariant&);

public:
    gnomonMesh *updatedMesh(void);

private:
    class gnomonFemSolverCommandPrivate *d;
    using gnomonAbstractCommand<gnomonAbstractFemSolver>::action;
};
