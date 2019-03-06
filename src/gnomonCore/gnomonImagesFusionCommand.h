#include "gnomonAbstractCommand.h"

#include "gnomonLandmark.h"

class gnomonImagesSerie;

class GNOMONCORE_EXPORT gnomonImagesFusionCommand : public gnomonAbstractCommand
{
public:
     gnomonImagesFusionCommand(void) = delete;
     gnomonImagesFusionCommand(const QString&);
    ~gnomonImagesFusionCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void addImagesSerie(gnomonImagesSerie *);

    virtual void setParameter(const QString&, const QVariant&);

    gnomonImagesSerie *output(void);

    void removeImagesSeries(void);

    void addLandmarks(const std::vector<gnomonLandmark>&);
    void removeLandmarks(void);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;
    class gnomonImagesFusionCommandPrivate *d;
};
