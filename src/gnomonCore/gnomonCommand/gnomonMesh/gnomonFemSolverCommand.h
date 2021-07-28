#include "gnomonCommand/gnomonAbstractCommand.h"

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
    void setMesh(gnomonMeshSeries *mesh);
    void setParameter(const QString&, const QVariant&);

    QMap<QString, dtkCoreParameter *> parameters(void) const;
    gnomonMeshSeries *updatedMesh(void);

public:
    static bool isEmpty(void);

private:
    class gnomonFemSolverCommandPrivate *d;
};
