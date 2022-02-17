#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonImageWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonImageWriterCommand(void);
    ~gnomonImageWriterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(gnomonAbstractDynamicForm *form) override;
    void setImage(gnomonImageSeries *image_series);
    void setAlgorithmName(const QString &) override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "imageWriter";
    static QStringList availablePlugins();

private:
    class gnomonImageWriterCommandPrivate *d;
};
