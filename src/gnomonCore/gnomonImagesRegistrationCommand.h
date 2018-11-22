#include "gnomonAbstractCommand.h"
#include "gnomonAbstractImagesRegistration.h"

class GNOMONCORE_EXPORT gnomonImagesRegistrationCommand : public gnomonAbstractCommand<gnomonAbstractImagesRegistration>
{
public:
    gnomonImagesRegistrationCommand() = delete;
    gnomonImagesRegistrationCommand(const QString&);
    virtual ~gnomonImagesRegistrationCommand();

public:
    void redo(void) override;
    void undo(void) override;

public:
    void addImage(dtkImage *);

    virtual QMap<QString, gnomonParameter*> parameters(void) const;

    virtual void setParameter(const QString&, const QVariant&);

    double time(void);
    dtkImage *at(double t);
    dtkImage *next(void);

private:
    class gnomonImagesRegistrationCommandPrivate *d;
};
