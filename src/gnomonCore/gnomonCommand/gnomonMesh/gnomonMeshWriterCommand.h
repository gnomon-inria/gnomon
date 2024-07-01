#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractWriterCommand>

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>

class GNOMONCORE_EXPORT gnomonMeshWriterCommand : public gnomonAbstractWriterCommand
{
public:
     gnomonMeshWriterCommand(void);
    ~gnomonMeshWriterCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) override;
    void setMesh(std::shared_ptr<gnomonMeshSeries> image_series);
    void setAlgorithmName(const QString& algo_name) override;

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    void clear(void) override;

public:
    QStringList extensions(void) override;
    orderedMap inputTypes() override;

public:
    inline static const QString groupName = "meshWriter";

private:
    class gnomonMeshWriterCommandPrivate *d;
};

GNOMON_COMMAND_TRAITS(gnomonMeshWriterCommand)