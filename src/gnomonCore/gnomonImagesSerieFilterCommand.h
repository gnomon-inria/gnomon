#include "gnomonAbstractCommand.h"
#include "gnomonAbstractImagesSerieFilter.h"

class GNOMONCORE_EXPORT gnomonImagesSerieFilterCommand : public gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>
{
public:
    gnomonImagesSerieFilterCommand() = delete;
    gnomonImagesSerieFilterCommand(const QString&);
    virtual ~gnomonImagesSerieFilterCommand();

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setImage(dtkImage *image);
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue);
    virtual QMap<QString, QVariant> parameters(void) const;

public:
    double time(void);
    dtkImage *at(double t);
    dtkImage *next(void);

private:
    class gnomonImagesSerieFilterCommandPrivate *d;
};
