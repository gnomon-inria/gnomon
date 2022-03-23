#include "gnomonMeshWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshWriter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonMeshWriterCommandPrivate
{
public:
    gnomonMeshSeries* mesh = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshWriterCommand::gnomonMeshWriterCommand() : d(new gnomonMeshWriterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::meshWriter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::meshWriter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonMeshWriterCommand::~gnomonMeshWriterCommand()
{
    delete d;
}

void gnomonMeshWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::meshWriter::pluginFactory().create(algo_name);
}

void gnomonMeshWriterCommand::predo(void)
{
    ((gnomonAbstractMeshWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractMeshWriter *) this->action)->setMesh(d->mesh);
}

void gnomonMeshWriterCommand::postdo(void)
{

}

void gnomonMeshWriterCommand::undo()
{
    ((gnomonAbstractMeshWriter *) this->action)->setPath("");
}

void gnomonMeshWriterCommand::setMesh(gnomonMeshSeries *mesh)
{
    d->mesh = mesh;
}

void gnomonMeshWriterCommand::setForm(gnomonAbstractDynamicForm *form)
{
    d->mesh = dynamic_cast<gnomonMeshSeries*>(form);
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshWriterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["mesh"] = d->mesh;
    return inputs;
}

bool gnomonMeshWriterCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonMeshWriterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonMeshWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("mesh", "gnomonMesh"));
    return input_types;
}

void gnomonMeshWriterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "mesh") {
        this->setMesh(dynamic_cast<gnomonMeshSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonMeshWriterCommand.cpp ends here
