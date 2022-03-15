#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>

#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>

class GNOMONCORE_EXPORT gnomonImageReaderCommand: public gnomonAbstractReaderCommand
{
public:
     gnomonImageReaderCommand(void);
    ~gnomonImageReaderCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    gnomonImageSeries *image();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "imageReader";
    static QStringList availablePlugins();

private:
    class gnomonImageReaderCommandPrivate *d;
};
