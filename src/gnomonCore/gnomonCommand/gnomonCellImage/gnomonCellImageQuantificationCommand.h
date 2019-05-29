#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"

class GNOMONCORE_EXPORT gnomonCellImageQuantificationCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageQuantificationCommand(void) = delete;
     gnomonCellImageQuantificationCommand(const QString&);
    ~gnomonCellImageQuantificationCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setImage(gnomonImageSeries *image);
    void setCellImage(gnomonCellImageSeries *cellimage);

    gnomonCellImageSeries *cellImage(void);
    gnomonDataFrameSeries *dataFrame(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonCellImageQuantificationCommandPrivate *d;
};
