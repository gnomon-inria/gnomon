#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>

#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

class GNOMONCORE_EXPORT gnomonTreeConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonTreeConstructorCommand(void);
    ~gnomonTreeConstructorCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonTreeSeries> output();

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    inline static const QString groupName = "treeConstructor";

private:
    class gnomonTreeConstructorCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonTreeConstructorCommand)