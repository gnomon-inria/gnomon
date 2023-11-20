#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAlgorithmCommand>

#include "gnomonCore/gnomonForm/gnomonImage/gnomonImage"
#include "gnomonCore/gnomonForm/gnomonDataDict/gnomonDataDict.h"

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageRegistrationCommand : public gnomonAbstractAlgorithmCommand
{
public:
     gnomonImageRegistrationCommand(void);
    ~gnomonImageRegistrationCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setImage(std::shared_ptr<gnomonImageSeries> );
    std::shared_ptr<gnomonImageSeries> image();
    void setInitialTransformation(std::shared_ptr<gnomonDataDictSeries>);
    std::shared_ptr<gnomonDataDictSeries> initialTransformation();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;
    orderedMap inputTypes() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    std::shared_ptr<gnomonImageSeries> output();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;
    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString &) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "imageRegistration";
    static QStringList availablePlugins();

private:
    class gnomonImageRegistrationCommandPrivate *d;
};
