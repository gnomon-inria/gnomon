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
    void setParameter(const QString& parameterName, const QVariant& parameterValue);

public:
    double time(void);
    dtkImage *at(double t);
    dtkImage *next(void);

    QMap<QString, QVariant> parameters(void) const;

private:
    class gnomonImagesSerieFilterCommandPrivate *d;
    using gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action;
};
