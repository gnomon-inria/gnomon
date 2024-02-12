#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>

#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>

class GNOMONCORE_EXPORT gnomonImageConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonImageConstructorCommand(void);
    ~gnomonImageConstructorCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonImageSeries> output();

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString &) override;

public:
    inline static const QString groupName = "imageConstructor";

private:
    class gnomonImageConstructorCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonImageConstructorCommand)