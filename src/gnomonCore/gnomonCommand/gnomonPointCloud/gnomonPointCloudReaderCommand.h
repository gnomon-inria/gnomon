#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonPointCloud;

class GNOMONCORE_EXPORT gnomonPointCloudReaderCommand : public gnomonAbstractCommand
{
public:
     gnomonPointCloudReaderCommand(void) = delete;
     gnomonPointCloudReaderCommand(const QString&);
    ~gnomonPointCloudReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonPointCloudSeries *pointCloud(void);

private:
    class gnomonPointCloudReaderCommandPrivate *d;
};
