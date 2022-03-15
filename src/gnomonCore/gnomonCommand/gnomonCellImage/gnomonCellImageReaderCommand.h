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
    gnomonCellImageSeries *cellImage();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "cellImageReader";
    static QStringList availablePlugins();

private:
    class gnomonCellImageReaderCommandPrivate *d;
};
