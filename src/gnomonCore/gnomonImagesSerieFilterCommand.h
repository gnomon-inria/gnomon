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
    void setImage(dtkImage *image);

    virtual QMap<QString, gnomonParameter*> parameters(void) const;
    virtual void setParameter(const QString&, const QVariant&);

public:
    double time(void);
    dtkImage *at(double t);
    dtkImage *next(void);

private:
    class gnomonImagesSerieFilterCommandPrivate *d;
    using gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action;
};
