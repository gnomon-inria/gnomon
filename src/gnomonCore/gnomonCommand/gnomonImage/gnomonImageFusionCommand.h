#include "gnomonCommand/gnomonAbstractCommand.h"

#include "gnomonLandmark.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageFusionCommand : public gnomonAbstractCommand
{
public:
     gnomonImageFusionCommand();
    ~gnomonImageFusionCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void addImage(gnomonImageSeries *);
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void setAlgorithmName(const QString &) override;

    gnomonImageSeries *output() const;
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void addLandmarks(const std::vector<gnomonLandmark>&);
    void removeLandmarks();

public:
    static bool isEmpty();

private:
    class gnomonImageFusionCommandPrivate *d;
};
