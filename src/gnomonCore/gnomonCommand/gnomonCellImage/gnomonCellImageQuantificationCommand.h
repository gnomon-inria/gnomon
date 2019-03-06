#include "gnomonCommand/gnomonAbstractCommand.h"
class gnomonCellImage;

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
    void setImage(gnomonImagesSerie *image);
    void setCellImage(gnomonCellImage *cellimage);

    gnomonCellImage *cellImage(void);
    gnomonDataFrame *dataFrame(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonCellImageQuantificationCommandPrivate *d;
};
