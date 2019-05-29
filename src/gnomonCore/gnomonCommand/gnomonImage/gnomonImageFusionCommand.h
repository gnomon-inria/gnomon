#include "gnomonCommand/gnomonAbstractCommand.h"

#include "gnomonLandmark.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageFusionCommand : public gnomonAbstractCommand
{
public:
     gnomonImageFusionCommand(void) = delete;
     gnomonImageFusionCommand(const QString&);
    ~gnomonImageFusionCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void addImage(gnomonImageSeries *);

    virtual void setParameter(const QString&, const QVariant&);

    gnomonImageSeries *output(void);

    void removeImages(void);

    void addLandmarks(const std::vector<gnomonLandmark>&);
    void removeLandmarks(void);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;
    class gnomonImageFusionCommandPrivate *d;
};
