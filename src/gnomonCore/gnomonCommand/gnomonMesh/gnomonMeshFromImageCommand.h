#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonMeshFromImageCommand : public gnomonAbstractCommand
{
public:
     gnomonMeshFromImageCommand(void);
    ~gnomonMeshFromImageCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonImageSeries *image_series);

    gnomonImageSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    gnomonMeshSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "meshFromImage";
    static QStringList availablePlugins();

private:
    class gnomonMeshFromImageCommandPrivate *d;
};
