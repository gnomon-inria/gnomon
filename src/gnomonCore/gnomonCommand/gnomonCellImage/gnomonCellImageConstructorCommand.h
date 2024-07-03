#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>

#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>

class GNOMONCORE_EXPORT gnomonCellImageConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonCellImageConstructorCommand();
    ~gnomonCellImageConstructorCommand() override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonCellImageSeries> output();

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void clear(void) override;

    void setAlgorithmName(const QString &) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "cellImageConstructor";

private:
    class gnomonCellImageConstructorCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonCellImageConstructorCommand)