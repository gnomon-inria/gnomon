#include "gnomonAbstractCommand.h"
#include "gnomonAbstractImagesSerieFilter.h"

class GNOMONCORE_EXPORT gnomonImagesSerieFilterCommand : public gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>
{
public:
     gnomonImagesSerieFilterCommand(void) = delete;
     gnomonImagesSerieFilterCommand(const QString&);
    ~gnomonImagesSerieFilterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonImagesSerie *images_serie);
    gnomonImagesSerie *input(void);

    gnomonImagesSerie *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonImagesSerieFilterCommandPrivate *d;
    using gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action;
};
