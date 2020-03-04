#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudQuantificationCommand : public gnomonAbstractCommand
{
public:
     gnomonPointCloudQuantificationCommand(void) = delete;
     gnomonPointCloudQuantificationCommand(const QString&);
    ~gnomonPointCloudQuantificationCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setImage(gnomonImageSeries *image);
    void setPointCloud(gnomonPointCloudSeries *pointCloud);

    gnomonPointCloudSeries *pointCloud(void);
    gnomonDataFrameSeries *dataFrame(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonPointCloudQuantificationCommandPrivate *d;
};
