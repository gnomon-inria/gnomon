#include "gnomonMeshConstructorCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshConstructor.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonMeshConstructorCommandPrivate
{
public:
    std::shared_ptr<gnomonMeshSeries> output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshConstructorCommand::gnomonMeshConstructorCommand() : d(new gnomonMeshConstructorCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonMeshConstructorCommand)

    QStringList keys = gnomonCore::meshConstructor::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::meshConstructor::pluginFactory().create(this->algorithm_name);
    }

}

gnomonMeshConstructorCommand::~gnomonMeshConstructorCommand()
{
    delete d;
}

void gnomonMeshConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::meshConstructor::pluginFactory().create(algo_name);
}

void gnomonMeshConstructorCommand::predo(void)
{

}

void gnomonMeshConstructorCommand::postdo(void)
{
    std::shared_ptr<gnomonMeshSeries> mesh = ((gnomonAbstractMeshConstructor *) this->action)->output();

    if ((!mesh)||(mesh->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = mesh;
    }
}

void gnomonMeshConstructorCommand::undo()
{
}

std::shared_ptr<gnomonMeshSeries> gnomonMeshConstructorCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonMeshConstructorCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonMeshConstructorCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonMesh"));
    return types;
}

void gnomonMeshConstructorCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonMeshSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonMeshConstructorCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

void gnomonMeshConstructorCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->output = nullptr;
}

//
// gnomonMeshConstructorCommand.cpp ends here
