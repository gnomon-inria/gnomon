#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand.h>
#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h>


class GNOMONCORE_EXPORT gnomonBinaryImageReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonBinaryImageReaderCommand();
    ~gnomonBinaryImageReaderCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonBinaryImageSeries> binaryImage();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "binaryImageReader";
    static QStringList availablePlugins();

private:
    class gnomonBinaryImageReaderCommandPrivate *d;
};
