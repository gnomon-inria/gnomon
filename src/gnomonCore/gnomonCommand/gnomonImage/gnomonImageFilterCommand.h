#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageFilterCommand : public gnomonAbstractCommand
{
public:
     gnomonImageFilterCommand(void) = delete;
     gnomonImageFilterCommand(const QString&);
    ~gnomonImageFilterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonImageSeries *image_series);
    gnomonImageSeries *input(void);

    gnomonImageSeries *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonImageFilterCommandPrivate *d;
};
