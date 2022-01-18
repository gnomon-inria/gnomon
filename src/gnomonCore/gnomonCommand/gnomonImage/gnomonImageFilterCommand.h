#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageFilterCommand : public gnomonAbstractCommand
{
public:
     gnomonImageFilterCommand(void);
    ~gnomonImageFilterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

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

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();

private:
    class gnomonImageFilterCommandPrivate *d;
};
