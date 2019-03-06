#include "gnomonCommand/gnomonAbstractCommand.h"

class gnomonImagesSerie;

class GNOMONCORE_EXPORT gnomonTreeReaderCommand : public gnomonAbstractCommand
{
public:
     gnomonTreeReaderCommand(void) = delete;
     gnomonTreeReaderCommand(const QString&);
    ~gnomonTreeReaderCommand(void);

public:
    void redo(void) override;
    void undo(void) override;

public:
    void setPath(const QString& path);

    gnomonTree *tree(void);

private:
    class gnomonTreeReaderCommandPrivate *d;
};
