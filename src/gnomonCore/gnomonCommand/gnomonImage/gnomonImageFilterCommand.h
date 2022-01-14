#include "gnomonCommand/gnomonAbstractCommand.h"

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

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "imageFilter";
    static QStringList availablePlugins();

private:
    class gnomonImageFilterCommandPrivate *d;
};
