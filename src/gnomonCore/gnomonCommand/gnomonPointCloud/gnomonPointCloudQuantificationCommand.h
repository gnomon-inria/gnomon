#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudQuantificationCommand : public gnomonAbstractAlgorithmCommand
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

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonPointCloudSeries *pointCloud(void);
    gnomonDataFrameSeries *dataFrame(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonPointCloudQuantificationCommandPrivate *d;
};
