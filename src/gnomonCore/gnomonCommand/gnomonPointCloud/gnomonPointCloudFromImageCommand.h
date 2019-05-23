#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImagesSerie;
class gnomonPointCloud;

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
    void setInput(gnomonImagesSerie *images_serie);
    gnomonImagesSerie *input(void);

    gnomonPointCloud *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonPointCloudFromImageCommandPrivate *d;
};
