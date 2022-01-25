#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonCellImage;

class GNOMONCORE_EXPORT gnomonCellImageFilterCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageFilterCommand();
    ~gnomonCellImageFilterCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonCellImageSeries *cellImage_series);
    gnomonCellImageSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    gnomonCellImageSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellImageFilter";
    static QStringList availablePlugins();

private:
    class gnomonCellImageFilterCommandPrivate *d;
};
