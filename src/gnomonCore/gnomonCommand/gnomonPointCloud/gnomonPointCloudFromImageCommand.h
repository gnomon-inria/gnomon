#include "gnomonCommand/gnomonAbstractCommand.h"

class GNOMONCORE_EXPORT gnomonPointCloudFromImageCommand : public gnomonAbstractCommand
{
public:
     gnomonPointCloudFromImageCommand(void);
    ~gnomonPointCloudFromImageCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonImageSeries *image);
    gnomonImageSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    gnomonPointCloudSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void setAlgorithmName(const QString& algo_name) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

public:
    static bool isEmpty();

private:
    class gnomonPointCloudFromImageCommandPrivate *d;
};
