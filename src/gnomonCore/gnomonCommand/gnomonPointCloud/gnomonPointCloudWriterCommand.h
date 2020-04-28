#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonPointCloudWriterCommand(void) = delete;
     gnomonPointCloudWriterCommand(const QString&);
    ~gnomonPointCloudWriterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);
    void setPointCloud(gnomonPointCloudSeries *pointCloud);

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonPointCloudWriterCommandPrivate *d;
};
