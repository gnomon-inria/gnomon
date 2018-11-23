#include "gnomonAbstractCommand.h"
#include "gnomonAbstractImagesFusion.h"

class GNOMONCORE_EXPORT gnomonImagesFusionCommand : public gnomonAbstractCommand<gnomonAbstractImagesFusion>
{
public:
     gnomonImagesFusionCommand(void) = delete;
     gnomonImagesFusionCommand(const QString&);
    ~gnomonImagesFusionCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void addImage(dtkImage *);
    void setParameter(const QString&, const QVariant&);

public:
    dtkImage *output(void);
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonImagesFusionCommandPrivate *d;
};
