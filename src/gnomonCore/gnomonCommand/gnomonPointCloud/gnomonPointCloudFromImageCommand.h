#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudFromImageCommand : public gnomonAbstractCommand
{
public:
     gnomonPointCloudFromImageCommand(void) = delete;
     gnomonPointCloudFromImageCommand(const QString&);
    ~gnomonPointCloudFromImageCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonImageSeries *image_series);
    gnomonImageSeries *input(void);

    gnomonPointCloudSeries *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonPointCloudFromImageCommandPrivate *d;
};
