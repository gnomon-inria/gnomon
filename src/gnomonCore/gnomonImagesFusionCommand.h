#include "gnomonAbstractCommand.h"
#include "gnomonAbstractImagesFusion.h"

#include "gnomonLandmark.h"

class GNOMONCORE_EXPORT gnomonImagesFusionCommand : public gnomonAbstractCommand<gnomonAbstractImagesFusion>
{
public:
     gnomonImagesFusionCommand(void) = delete;
     gnomonImagesFusionCommand(const QString&);
    ~gnomonImagesFusionCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setParameter(const QString&, const QVariant&);

    void addImage(dtkImage *);
    void removeImages(void);

    void addLandmarks(const std::vector<gnomonLandmark>&);
    void removeLandmarks(void);

public:
    dtkImage *output(void);
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonImagesFusionCommandPrivate *d;
};
