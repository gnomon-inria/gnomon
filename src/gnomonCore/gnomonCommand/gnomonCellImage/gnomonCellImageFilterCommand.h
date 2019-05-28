#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"

class GNOMONCORE_EXPORT gnomonCellImageFilterCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageFilterCommand(void) = delete;
     gnomonCellImageFilterCommand(const QString&);
    ~gnomonCellImageFilterCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonCellImageSeries *images_serie);
    gnomonCellImageSeries *input(void);

    gnomonCellImageSeries *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonCellImageFilterCommandPrivate *d;
};
