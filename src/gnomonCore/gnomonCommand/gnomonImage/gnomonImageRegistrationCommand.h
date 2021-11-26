#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageRegistrationCommand : public gnomonAbstractCommand
{
public:
    gnomonImageRegistrationCommand();
    virtual ~gnomonImageRegistrationCommand();

public:
    void redo(void) override;
    void undo(void) override;

public:
    void addImage(gnomonImageSeries *);
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;
    virtual orderedMap inputTypes(void) override;
    virtual void addInputForm(gnomonAbstractDynamicForm *form) override;

    gnomonImageSeries *output();
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;
    virtual orderedMap outputTypes(void) override;

    dtkCoreParameters parameters(void) const override;
    void setParameter(const QString&, const QVariant&);
    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty(void);

private:
    class gnomonImageRegistrationCommandPrivate *d;
};
