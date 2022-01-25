#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonMeshReaderCommand(void);
    ~gnomonMeshReaderCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    gnomonMeshSeries *mesh();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "meshReader";
    static QStringList availablePlugins();

private:
    class gnomonMeshReaderCommandPrivate *d;
};
