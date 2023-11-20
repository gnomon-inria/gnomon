#pragma once

#include <gnomonCoreExport>

#include "gnomonCommand/gnomonAbstractAlgorithmCommand.h"
#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class GNOMONCORE_EXPORT gnomonBinaryImageFromImageCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonBinaryImageFromImageCommand();
    ~gnomonBinaryImageFromImageCommand() override;

public:
     void predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(std::shared_ptr<gnomonImageSeries> image);
    std::shared_ptr<gnomonImageSeries> input();
    void setInitialization(std::shared_ptr<gnomonBinaryImageSeries> init);
    std::shared_ptr<gnomonBinaryImageSeries> initialization();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    orderedMap inputTypes() override;
    void setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    std::shared_ptr<gnomonBinaryImageSeries> output();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;
    orderedMap outputTypes() override;

    void setAlgorithmName(const QString &) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "binaryImageFromImage";
    static QStringList availablePlugins();

private:
    class gnomonBinaryImageFromImageCommandPrivate *d;
};
