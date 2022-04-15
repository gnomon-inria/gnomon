#include "gnomonFemSolverCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractFemSolver.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonFemSolverCommandPrivate
{
public:
    QVariantMap parameters;

public:
    gnomonMeshSeries* mesh = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonFemSolverCommand::gnomonFemSolverCommand() : d(new gnomonFemSolverCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::femSolver::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::femSolver::pluginFactory().create(this->algorithm_name);
    }
}

gnomonFemSolverCommand::~gnomonFemSolverCommand()
{
    delete d;
}

void gnomonFemSolverCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::femSolver::pluginFactory().create(algo_name);
}

void gnomonFemSolverCommand::predo(void)
{
    ((gnomonAbstractFemSolver *) this->action)->setMesh(d->mesh);
}

void gnomonFemSolverCommand::postdo(void)
{

}

void gnomonFemSolverCommand::undo()
{
    ((gnomonAbstractFemSolver *) this->action)->setMesh(nullptr);
}

void gnomonFemSolverCommand::setMesh(gnomonMeshSeries *mesh)
{
    d->mesh = mesh;
    qDebug()<<"setmesh"<<d->mesh;
}

gnomonMeshSeries *gnomonFemSolverCommand::updatedMesh()
{
    return ((gnomonAbstractFemSolver *) this->action)->updatedMesh();
}

bool gnomonFemSolverCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonFemSolverCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonFemSolverCommand::inputs() {
    QMap<QString, gnomonAbstractDynamicForm*> inputs;
    inputs["inputMesh"] = this->inputMesh();
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonFemSolverCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("inputMesh", "gnomonMesh"));
    return input_types;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonFemSolverCommand::outputs() {
    QMap<QString, gnomonAbstractDynamicForm*> outputs;
    outputs["updatedMesh"] = this->updatedMesh();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonFemSolverCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("updatedMesh", "gnomonMesh"));
    return output_types;
}

gnomonMeshSeries *gnomonFemSolverCommand::inputMesh() {
    return this->d->mesh;
}

void gnomonFemSolverCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "inputMesh") {
        this->setMesh(dynamic_cast<gnomonMeshSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonFemSolverCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->mesh) {
        d->mesh = new gnomonMeshSeries();
    }
    auto tmp = serialization["updatedMesh"].toObject();
    d->mesh->deserialize(tmp);
}

QJsonObject gnomonFemSolverCommand::serializeResults(void) {
    QJsonObject out;
    out["updatedMesh"] = d->mesh->serialize();
    return out;
}

//
// gnomonFemSolverCommand.cpp ends here
