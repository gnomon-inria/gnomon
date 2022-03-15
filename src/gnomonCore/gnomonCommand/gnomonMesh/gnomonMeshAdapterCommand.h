#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractAdapterCommand>

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshAdapterCommand : public gnomonAbstractAdapterCommand
{
public:
     gnomonMeshAdapterCommand(void);
    ~gnomonMeshAdapterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setInput(gnomonMeshSeries *mesh_series);
    void setAlgorithmName(const QString & algo) override;
    gnomonMeshSeries *input();
    QMap<QString, gnomonAbstractDynamicForm *> inputs() override;

    void setInputForm(const QString &name, gnomonAbstractDynamicForm *form) override;

    orderedMap inputTypes() override;

    orderedMap outputTypes() override;

    gnomonAbstractDynamicForm *output();
    QMap<QString, gnomonAbstractDynamicForm *> outputs() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    static bool isEmpty();
    inline static const QString groupName = "meshAdapter";
    static QStringList availablePlugins();

private:
    class gnomonMeshAdapterCommandPrivate *d;
};
