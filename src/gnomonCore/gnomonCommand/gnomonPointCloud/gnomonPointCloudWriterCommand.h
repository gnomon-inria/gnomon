#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonPointCloudWriterCommand(void);
    ~gnomonPointCloudWriterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(gnomonAbstractDynamicForm *form) override;
    void setPointCloud(gnomonPointCloudSeries *pointCloud);
    void setAlgorithmName(const QString& algo_name) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

public:
    QStringList extensions(void) override;
    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "pointCloudWriter";
    static QStringList availablePlugins();

private:
    class gnomonPointCloudWriterCommandPrivate *d;
};
