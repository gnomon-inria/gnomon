#include "gnomonCommand/gnomonAbstractCommand.h"


class GNOMONCORE_EXPORT gnomonBinaryImageFromImageCommand : public gnomonAbstractCommand
{
public:
     gnomonBinaryImageFromImageCommand();
    ~gnomonBinaryImageFromImageCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setInput(gnomonImageSeries *image);
    gnomonImageSeries *input();
    void setInitialization(gnomonBinaryImageSeries *init);
    gnomonBinaryImageSeries *initialization();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;
    void setInputForm(const QString& name, gnomonAbstractDynamicForm *form) override;

    gnomonBinaryImageSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;
    orderedMap outputTypes() override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();

private:
    class gnomonBinaryImageFromImageCommandPrivate *d;
};
