#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>

class GNOMONCORE_EXPORT gnomonCellImageReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonCellImageReaderCommand();
    ~gnomonCellImageReaderCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonCellImageSeries> cellImage();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void clear(void) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "cellImageReader";

private:
    class gnomonCellImageReaderCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonCellImageReaderCommand)