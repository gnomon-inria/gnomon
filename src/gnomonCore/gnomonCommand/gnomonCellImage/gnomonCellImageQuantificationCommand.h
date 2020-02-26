#include "gnomonCommand/gnomonAbstractCommand.h"

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

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageQuantificationCommandPrivate *d;
};
