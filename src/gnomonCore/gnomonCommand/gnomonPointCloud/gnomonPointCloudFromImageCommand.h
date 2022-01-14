#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudFromImageCommand : public gnomonAbstractCommand
{
public:
     gnomonPointCloudFromImageCommand();
    ~gnomonPointCloudFromImageCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setInput(gnomonImageSeries *image);
    gnomonImageSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    gnomonPointCloudSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void setAlgorithmName(const QString& algo_name) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "pointCloudFromImage";
    static QStringList availablePlugins();

private:
    class gnomonPointCloudFromImageCommandPrivate *d;
};
