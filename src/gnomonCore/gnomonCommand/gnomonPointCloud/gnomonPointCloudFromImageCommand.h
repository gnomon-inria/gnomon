#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudFromImageCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonPointCloudFromImageCommand(void) = delete;
     gnomonPointCloudFromImageCommand(const QString&);
    ~gnomonPointCloudFromImageCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setInput(gnomonImageSeries *image);
    gnomonImageSeries *input(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    gnomonPointCloudSeries *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);

public:
    QMap<QString, dtkCoreParameter *> parameters(void) const override;

public:
    static bool isEmpty(void);

private:
    class gnomonPointCloudFromImageCommandPrivate *d;
};
