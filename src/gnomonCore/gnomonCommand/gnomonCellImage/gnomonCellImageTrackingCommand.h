#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageTrackingCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageTrackingCommand(void) = delete;
     gnomonCellImageTrackingCommand(const QString&);
    ~gnomonCellImageTrackingCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setImage(gnomonImageSeries *image);
    void setCellImage(gnomonCellImageSeries *cellimage);

    gnomonCellImageSeries *cellImage(void);
    gnomonTreeSeries *tree(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonCellImageTrackingCommandPrivate *d;
};
