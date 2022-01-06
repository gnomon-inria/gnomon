#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonCellImageFromImageCommand : public gnomonAbstractCommand
{
public:
     gnomonCellImageFromImageCommand();
    ~gnomonCellImageFromImageCommand() override;

public:
    void redo() override;
    void undo() override;

public:
    void setInput(gnomonImageSeries *image_series);
    gnomonImageSeries *input();

public:
    void setCellPoints(gnomonPointCloudSeries *pointCloud_series);
    gnomonPointCloudSeries *cellPoints();
    void setBinaryImage(gnomonBinaryImageSeries *binary_image_series);
    gnomonBinaryImageSeries *binaryImage();;

    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;
    orderedMap inputTypes() override;
    void setInputForm(const QString& name, gnomonAbstractDynamicForm *form) override;

public:
    void setAlgorithmName(const QString &) override;

public:
    gnomonCellImageSeries *output();

    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;
    orderedMap outputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonCellImageFromImageCommandPrivate *d;
};

//
// gnomonCellImageFromImageCommand.h ends here
