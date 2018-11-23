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
    void addImagesSerie(gnomonImagesSerie *);

    virtual QMap<QString, gnomonParameter*> parameters(void) const;

    virtual void setParameter(const QString&, const QVariant&);

    gnomonImagesSerie *output(void);

private:
    class gnomonImagesFusionCommandPrivate *d;
};
