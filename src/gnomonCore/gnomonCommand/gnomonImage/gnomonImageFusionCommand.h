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
    void addImage(gnomonImageSeries *);
    void changeImage(const QString&, gnomonImageSeries *);
    void reloadImages();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    void setAlgorithmName(const QString &) override;

    gnomonImageSeries *output() const;
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

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
