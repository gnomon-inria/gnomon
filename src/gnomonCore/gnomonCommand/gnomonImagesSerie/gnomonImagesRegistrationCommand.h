#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImagesSerie;

class GNOMONCORE_EXPORT gnomonImagesRegistrationCommand : public gnomonAbstractCommand
{
public:
    gnomonImagesRegistrationCommand() = delete;
    gnomonImagesRegistrationCommand(const QString&);
    virtual ~gnomonImagesRegistrationCommand();

public:
    void redo(void) override;
    void undo(void) override;

public:
    void addImagesSerie(gnomonImagesSerie *);

    gnomonImagesSerie *output();
    
    virtual QMap<QString, gnomonCoreParameter*> parameters(void) const;

    virtual void setParameter(const QString&, const QVariant&);

private:
    class gnomonImagesRegistrationCommandPrivate *d;
};
