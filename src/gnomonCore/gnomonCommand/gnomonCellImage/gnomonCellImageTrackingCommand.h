#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageTrackingCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageTrackingCommand(void);
    ~gnomonCellImageTrackingCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setImage(gnomonImageSeries *image);
    void setCellImage(gnomonCellImageSeries *cellimage);

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    gnomonCellImageSeries *cellImage();
    gnomonTreeSeries *tree();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellImageTracking";
    static QStringList availablePlugins();

private:
    class gnomonCellImageTrackingCommandPrivate *d;
};
