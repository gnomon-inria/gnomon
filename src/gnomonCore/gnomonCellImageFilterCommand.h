#include "gnomonAbstractCommand.h"
class gnomonCellImage;

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
    void setInput(gnomonCellImage *images_serie);
    gnomonCellImage *input(void);

    gnomonCellImage *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonCellImageFilterCommandPrivate *d;
};
