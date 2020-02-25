#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshFilterCommand : public gnomonAbstractCommand
{
public:
     gnomonMeshFilterCommand(void) = delete;
     gnomonMeshFilterCommand(const QString&);
    ~gnomonMeshFilterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonMeshSeries *mesh_series);
    gnomonMeshSeries *input(void);

    gnomonMeshSeries *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonMeshFilterCommandPrivate *d;
};
