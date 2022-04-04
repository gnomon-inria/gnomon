#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>

#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>

class gnomonImage;

class GNOMONCORE_EXPORT gnomonImageRegistrationCommand : public gnomonAbstractCommand
{
public:
     gnomonImageRegistrationCommand(void);
    ~gnomonImageRegistrationCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setImage(gnomonImageSeries *);
    gnomonImageSeries *image();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;
    orderedMap inputTypes() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    gnomonImageSeries *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;
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
