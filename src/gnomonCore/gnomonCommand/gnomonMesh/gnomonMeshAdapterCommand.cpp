#include "gnomonMeshAdapterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshAdapter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonMeshAdapterCommandPrivate
{
public:
    std::shared_ptr<gnomonMeshSeries> input = nullptr;
    std::shared_ptr<gnomonAbstractDynamicForm> output = nullptr;
};

gnomonMeshAdapterCommand::gnomonMeshAdapterCommand() : d(new gnomonMeshAdapterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonMeshAdapterCommand)

    QStringList keys = gnomonCore::meshAdapter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::meshAdapter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonMeshAdapterCommand::~gnomonMeshAdapterCommand()
{
    delete d;
}

void gnomonMeshAdapterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::meshAdapter::pluginFactory().create(algo_name);
}

void gnomonMeshAdapterCommand::predo(void)
{

}

void gnomonMeshAdapterCommand::postdo(void)
{
    std::shared_ptr<gnomonAbstractDynamicForm> output = ((gnomonAbstractMeshAdapter *) this->action)->output();

    if ((!output)||(output->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonMeshAdapterCommand::undo()
{
    ((gnomonAbstractMeshAdapter *) this->action)->setInput(nullptr);
    this->action->refreshParameters();
}

void gnomonMeshAdapterCommand::setInput(std::shared_ptr<gnomonMeshSeries> input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractMeshAdapter *) this->action)->setInput(d->input);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonMeshSeries> gnomonMeshAdapterCommand::input()
{
    return d->input;
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonMeshAdapterCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonMeshAdapterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonMeshAdapterCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonMeshAdapterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonMesh"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonMeshAdapterCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonAbstractDynamicForm"));
    return output_types;
}

void gnomonMeshAdapterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonMeshSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonMeshAdapterCommand::deserializeResults(QJsonObject &serialization) {
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonMeshAdapterCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

void gnomonMeshAdapterCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->input = nullptr;
    d->output = nullptr;
}

//
// gnomonMeshAdapterCommand.cpp ends here
