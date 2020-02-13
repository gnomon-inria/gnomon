#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonMeshFromImageCommand : public gnomonAbstractCommand
{
public:
     gnomonMeshFromImageCommand(void) = delete;
     gnomonMeshFromImageCommand(const QString&);
    ~gnomonMeshFromImageCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonImageSeries *image_series);
    gnomonImageSeries *input(void);

    gnomonMeshSeries *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

private:
    class gnomonMeshFromImageCommandPrivate *d;
};
