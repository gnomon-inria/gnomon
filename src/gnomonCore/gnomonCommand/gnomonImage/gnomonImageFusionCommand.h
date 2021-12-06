#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

#include "gnomonLandmark.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageFusionCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonImageFusionCommand(void);
    ~gnomonImageFusionCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void addImage(gnomonImageSeries *);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

    virtual void setParameter(const QString&, const QVariant&);
    void setAlgorithmName(const QString &) override;

    gnomonImageSeries *output(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

    void addLandmarks(const std::vector<gnomonLandmark>&);
    void removeLandmarks(void);

public:
    static bool isEmpty(void);

public:
    dtkCoreParameters parameters(void) const override;
    class gnomonImageFusionCommandPrivate *d;
};
