#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>

#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageFilterCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonImageFilterCommand(void);
    ~gnomonImageFilterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(std::shared_ptr<gnomonImageSeries> image_series);
    std::shared_ptr<gnomonImageSeries> input();

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;
    orderedMap inputTypes() override;
    void setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    std::shared_ptr<gnomonBinaryImageSeries> mask();
    void setMask(std::shared_ptr<gnomonBinaryImageSeries> init);

    std::shared_ptr<gnomonImageSeries> output();

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;
    orderedMap outputTypes() override;

    void clear(void) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString &) override;

public:
    inline static const QString groupName = "imageFilter";

private:
    class gnomonImageFilterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonImageFilterCommand)