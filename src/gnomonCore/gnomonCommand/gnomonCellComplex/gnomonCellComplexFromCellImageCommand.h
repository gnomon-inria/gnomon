#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonCellComplex;
class gnomonCellImage;

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
    void setInput(gnomonCellImage *images_serie);

    gnomonCellImage *input(void);
    gnomonCellComplex *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonCellComplexFromCellImageCommandPrivate *d;
};
