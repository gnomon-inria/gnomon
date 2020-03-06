#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonImageReaderCommand : public gnomonAbstractCommand
{
public:
     gnomonImageReaderCommand(void) = delete;
     gnomonImageReaderCommand(const QString&);
    ~gnomonImageReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonImageSeries *image(void);
    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs(void) override;

public:
    static bool isEmpty(void);

public:
    static const QString factoryName(void);

private:
    class gnomonImageReaderCommandPrivate *d;
};
