#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageRegistrationCommand : public gnomonAbstractCommand
{
public:
    gnomonImageRegistrationCommand();
    ~gnomonImageRegistrationCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void addImage(gnomonImageSeries *);
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;
    orderedMap inputTypes() override;
    void addInputForm(gnomonAbstractDynamicForm *form) override;

    gnomonImageSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;
    orderedMap outputTypes() override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();

private:
    class gnomonImageRegistrationCommandPrivate *d;
};
