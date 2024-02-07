#include "gnomonTreeAdapterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeAdapter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonTreeAdapterCommandPrivate
{
public:
    std::shared_ptr<gnomonTreeSeries> input = nullptr;
    std::shared_ptr<gnomonAbstractDynamicForm> output = nullptr;
};

gnomonTreeAdapterCommand::gnomonTreeAdapterCommand() : d(new gnomonTreeAdapterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonTreeAdapterCommand)

    QStringList keys = gnomonCore::treeAdapter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeAdapter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonTreeAdapterCommand::~gnomonTreeAdapterCommand()
{
    delete d;
}

void gnomonTreeAdapterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::treeAdapter::pluginFactory().create(algo_name);
}

void gnomonTreeAdapterCommand::predo(void)
{

}

void gnomonTreeAdapterCommand::postdo(void)
{   
    std::shared_ptr<gnomonAbstractDynamicForm> output = ((gnomonAbstractTreeAdapter *) this->action)->output();

    if ((!output)||(output->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonTreeAdapterCommand::undo()
{
    ((gnomonAbstractTreeAdapter *) this->action)->setInput(nullptr);
    this->action->refreshParameters();
}

void gnomonTreeAdapterCommand::setInput(std::shared_ptr<gnomonTreeSeries> input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
    }
    Q_ASSERT(this->action);
    if (d->input) {
        d->input->load();
    }
    ((gnomonAbstractTreeAdapter *) this->action)->setInput(d->input);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonTreeSeries> gnomonTreeAdapterCommand::input()
{
    return d->input;
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonTreeAdapterCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonTreeAdapterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonTreeAdapterCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonTreeAdapterCommand::inputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("input", "gnomonTree"));
    return types;
}

gnomonAbstractCommand::orderedMap gnomonTreeAdapterCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonAbstractDynamicForm"));
    return types;
}

void gnomonTreeAdapterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonTreeSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonTreeAdapterCommand::deserializeResults(QJsonObject &serialization) {
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonTreeAdapterCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

void gnomonTreeAdapterCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->input = nullptr;
    d->output = nullptr;
}

//
// gnomonTreeAdapterCommand.cpp ends here
