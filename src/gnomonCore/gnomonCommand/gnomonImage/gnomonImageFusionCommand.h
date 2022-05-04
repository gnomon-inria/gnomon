#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>

#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>

#include "gnomonLandmark.h"

class GNOMONCORE_EXPORT gnomonImageFusionCommand : public gnomonAbstractCommand
{
public:
     gnomonImageFusionCommand(void);
    ~gnomonImageFusionCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void addImage(std::shared_ptr<gnomonImageSeries> );
    void changeImage(const QString&, std::shared_ptr<gnomonImageSeries> );
    void reloadImages();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    void setAlgorithmName(const QString &) override;

    std::shared_ptr<gnomonImageSeries> output() const;
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void addLandmarks(const std::vector<gnomonLandmark>&);
    void removeLandmarks();

public:
    static bool isEmpty();
    inline static const QString groupName = "imageFusion";
    static QStringList availablePlugins();

private:
    class gnomonImageFusionCommandPrivate *d;
};
