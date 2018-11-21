#include "gnomonAbstractCommand.h"
#include "gnomonAbstractImagesFusion.h"

class GNOMONCORE_EXPORT gnomonImagesFusionCommand : public gnomonAbstractCommand<gnomonAbstractImagesFusion>
{
public:
    gnomonImagesFusionCommand() = delete;
    gnomonImagesFusionCommand(const QString&);
    virtual ~gnomonImagesFusionCommand();

public:
    void redo(void) override;
    void undo(void) override;

public:
    void addImage(dtkImage *);

    virtual QMap<QString, gnomonParameter*> parameters(void) const;

    virtual void setParameter(const QString&, const QVariant&);

    dtkImage *output(void);

private:
    class gnomonImagesFusionCommandPrivate *d;
};
