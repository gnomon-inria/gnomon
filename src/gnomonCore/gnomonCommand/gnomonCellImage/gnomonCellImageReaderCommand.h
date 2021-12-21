#include "gnomonCommand/gnomonAbstractCommand.h"
#include "gnomonCommand/gnomonAbstractReaderCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonCellImageReaderCommand();
    ~gnomonCellImageReaderCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    gnomonCellImageSeries *cellImage();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonCellImageReaderCommandPrivate *d;
};
