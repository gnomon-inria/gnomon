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
    void setParameters(const QMap<QString, QVariant>&);
    void setParameter(const QString& parameterName, const QVariant& parameterValue);
    void addImage(dtkImage *);
    dtkImage *output(void);

private:
    class gnomonImagesFusionCommandPrivate *d;
};
