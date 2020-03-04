#include "gnomonCommand/gnomonAbstractCommand.h"

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
    void setInput(gnomonCellImageSeries *image_series);

    gnomonCellImageSeries *input(void);
    gnomonCellComplexSeries *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonCellComplexFromCellImageCommandPrivate *d;
};
