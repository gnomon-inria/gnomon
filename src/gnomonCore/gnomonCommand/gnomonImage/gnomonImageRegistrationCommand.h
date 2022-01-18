#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageRegistrationCommand : public gnomonAbstractCommand
{
public:
     gnomonImageRegistrationCommand(void);
    ~gnomonImageRegistrationCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    [[deprecated]] void addImage(gnomonImageSeries *);
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;
    orderedMap inputTypes() override;
    [[deprecated]] void addInputForm(gnomonAbstractDynamicForm *form) override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    gnomonImageSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;
    orderedMap outputTypes() override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();

private:
    class gnomonImageRegistrationCommandPrivate *d;
};
