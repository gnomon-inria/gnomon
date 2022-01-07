#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonPointCloudWriterCommand();
    ~gnomonPointCloudWriterCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setForm(gnomonAbstractDynamicForm *form) override;
    void setPointCloud(gnomonPointCloudSeries *pointCloud);
    void setAlgorithmName(const QString& algo_name) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

public:
    static bool isEmpty();

private:
    class gnomonPointCloudWriterCommandPrivate *d;
};
