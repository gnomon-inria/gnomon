#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"

class GNOMONCORE_EXPORT gnomonBinaryImageWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonBinaryImageWriterCommand();
    ~gnomonBinaryImageWriterCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(gnomonAbstractDynamicForm *) override;
    void setBinaryImage(gnomonBinaryImageSeries *binaryImage);
    void setAlgorithmName(const QString &) override;

    orderedMap inputTypes() override;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "binaryImageWriter";
    static QStringList availablePlugins();

private:
    class gnomonBinaryImageWriterCommandPrivate *d;
};
