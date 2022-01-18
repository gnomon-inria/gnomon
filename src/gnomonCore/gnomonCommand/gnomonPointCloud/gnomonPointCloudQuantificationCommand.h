#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudQuantificationCommand : public gnomonAbstractCommand
{
public:
     gnomonPointCloudQuantificationCommand(void);
    ~gnomonPointCloudQuantificationCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setImage(gnomonImageSeries *image);
    void setPointCloud(gnomonPointCloudSeries *pointCloud);
    void setAlgorithmName(const QString &) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    gnomonPointCloudSeries *pointCloud();
    gnomonDataFrameSeries *dataFrame();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonPointCloudQuantificationCommandPrivate *d;
};
