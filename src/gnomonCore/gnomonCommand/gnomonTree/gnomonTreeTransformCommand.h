#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonTree;

class GNOMONCORE_EXPORT gnomonTreeTransformCommand : public gnomonAbstractCommand
{
public:
     gnomonTreeTransformCommand(void) = delete;
     gnomonTreeTransformCommand(const QString&);
    ~gnomonTreeTransformCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonTreeSeries *image_series);
    gnomonTreeSeries *input(void);

    gnomonTreeSeries *output(void);

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, gnomonCoreParameter *> parameters(void) const;

public:
    static bool isEmpty(void);

private:
    class gnomonTreeTransformCommandPrivate *d;
};
