#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageFilterCommand : public gnomonAbstractCommand
{
public:
     gnomonImageFilterCommand();
    ~gnomonImageFilterCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setInput(gnomonImageSeries *image_series);
    gnomonImageSeries *input();

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;
    orderedMap inputTypes() override;
    void setInputForm(const QString& name, gnomonAbstractDynamicForm *form) override;
    void addInputForm(gnomonAbstractDynamicForm *form) override;

    gnomonBinaryImageSeries *mask();
    void setMask(gnomonBinaryImageSeries *init);


    gnomonImageSeries *output();

    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;
    orderedMap outputTypes() override;

    virtual void setParameter(const QString&, const QVariant&);
    void setAlgorithmName(const QString &) override;

public:
    [[nodiscard]] dtkCoreParameters parameters() const override;

public:
    static bool isEmpty();

private:
    class gnomonImageFilterCommandPrivate *d;
};
