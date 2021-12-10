#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractWriterCommand.h"


class GNOMONCORE_EXPORT gnomonBinaryImageWriterCommand : public gnomonAbstractWriterCommand
{
public:
    gnomonBinaryImageWriterCommand(void);
    ~gnomonBinaryImageWriterCommand();

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setForm(gnomonAbstractDynamicForm *) override;
    void setBinaryImage(gnomonBinaryImageSeries * binaryImage);
    void setAlgorithmName(const QString &) override;

    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs(void) override;

public:
    static bool isEmpoty(void);

private:
    class gnomonBinaryImageWriterCommandPrivate *d;

};