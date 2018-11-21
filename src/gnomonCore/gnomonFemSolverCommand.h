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
    void setParameter(const QString& parameterName, const QVariant& parameterValue);

    QMap<QString, QVariant> parameters(void) const;

public:
    gnomonMesh *updatedMesh(void);

private:
    class gnomonFemSolverCommandPrivate *d;
    using gnomonAbstractCommand<gnomonAbstractFemSolver>::action;
};
