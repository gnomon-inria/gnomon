#include "gnomonCommand/gnomonAbstractCommand.h"

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
    void setInput(gnomonCellImageSeries *image_series);
    gnomonCellImageSeries *input(void);

    gnomonCellImageSeries *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageFilterCommandPrivate *d;
};
