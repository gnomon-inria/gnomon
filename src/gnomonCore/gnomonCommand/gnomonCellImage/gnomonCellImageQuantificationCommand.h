#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageQuantificationCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageQuantificationCommand();
    ~gnomonCellImageQuantificationCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setImage(gnomonImageSeries *image);
    void setCellImage(gnomonCellImageSeries *cellimage);

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    gnomonCellImageSeries *cellImage();
    gnomonDataFrameSeries *dataFrame();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellImageQuantification";
    static QStringList availablePlugins();

private:
    class gnomonCellImageQuantificationCommandPrivate *d;
};
