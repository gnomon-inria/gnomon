#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonBinaryImageReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonBinaryImageReaderCommand(void);
    ~gnomonBinaryImageReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonBinaryImageSeries *binaryImage(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    static bool isEmpty(void);

private:
    class gnomonBinaryImageReaderCommandPrivate *d;
};
