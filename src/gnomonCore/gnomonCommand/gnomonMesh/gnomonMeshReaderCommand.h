#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractReaderCommand>

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class gnomonMesh;

class GNOMONCORE_EXPORT gnomonMeshReaderCommand : public gnomonAbstractReaderCommand
{
public:
     gnomonMeshReaderCommand(void);
    ~gnomonMeshReaderCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    std::shared_ptr<gnomonMeshSeries> mesh();
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    orderedMap outputTypes() override;

    void clear(void) override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

public:
    inline static const QString groupName = "meshReader";

private:
    class gnomonMeshReaderCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonMeshReaderCommand)