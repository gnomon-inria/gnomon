#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudWriterCommand : public gnomonAbstractCommand
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

private:
    class gnomonPointCloudWriterCommandPrivate *d;
};
