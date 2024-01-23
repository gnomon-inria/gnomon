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
    std::shared_ptr<gnomonImageSeries> image();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void clear(void) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "imageReader";

private:
    class gnomonImageReaderCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonImageReaderCommand)