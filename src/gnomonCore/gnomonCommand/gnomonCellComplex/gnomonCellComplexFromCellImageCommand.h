#include "gnomonCommand/gnomonAbstractCommand.h"

#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonCellComplex/gnomonCellComplex.h"

class GNOMONCORE_EXPORT gnomonCellComplexFromCellImageCommand : public gnomonAbstractCommand
{
public:
     gnomonCellComplexFromCellImageCommand(void) = delete;
     gnomonCellComplexFromCellImageCommand(const QString&);
    ~gnomonCellComplexFromCellImageCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonCellImageSeries *images_serie);

    gnomonCellImageSeries *input(void);
    gnomonCellComplexSeries *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonCellComplexFromCellImageCommandPrivate *d;
};
