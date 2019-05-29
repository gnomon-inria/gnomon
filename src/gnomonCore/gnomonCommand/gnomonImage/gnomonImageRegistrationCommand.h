#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageRegistrationCommand : public gnomonAbstractCommand
{
public:
    gnomonImageRegistrationCommand() = delete;
    gnomonImageRegistrationCommand(const QString&);
    virtual ~gnomonImageRegistrationCommand();

public:
    void redo(void) override;
    void undo(void) override;

public:
    void addImage(gnomonImageSeries *);

    gnomonImageSeries *output();
    
    virtual QMap<QString, gnomonCoreParameter*> parameters(void) const;

    virtual void setParameter(const QString&, const QVariant&);

private:
    class gnomonImageRegistrationCommandPrivate *d;
};
