#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractConstructorCommand>

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class GNOMONCORE_EXPORT gnomonMeshConstructorCommand : public gnomonAbstractConstructorCommand
{
public:
     gnomonMeshConstructorCommand(void);
    ~gnomonMeshConstructorCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonMeshSeries> output();

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

    void setAlgorithmName(const QString& algo_name) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "meshConstructor";
    static QStringList availablePlugins();

private:
    class gnomonMeshConstructorCommandPrivate *d;
};
