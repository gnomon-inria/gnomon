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
    
    QMap<QString, gnomonCoreParameter*> parameters(void) const;
    void setParameter(const QString&, const QVariant&);

public:
    static bool isEmpty(void);

private:
    class gnomonImageRegistrationCommandPrivate *d;
};
