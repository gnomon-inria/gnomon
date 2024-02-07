#include "gnomonLStringAdapterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringAdapter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonLStringAdapterCommandPrivate
{
public:
    std::shared_ptr<gnomonLStringSeries> input = nullptr;
    std::shared_ptr<gnomonAbstractDynamicForm> output = nullptr;
};

gnomonLStringAdapterCommand::gnomonLStringAdapterCommand() : d(new gnomonLStringAdapterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonLStringAdapterCommand)

    QStringList keys = gnomonCore::lStringAdapter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::lStringAdapter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonLStringAdapterCommand::~gnomonLStringAdapterCommand()
{
    delete d;
}

void gnomonLStringAdapterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::lStringAdapter::pluginFactory().create(algo_name);
}

void gnomonLStringAdapterCommand::predo(void)
{

}

void gnomonLStringAdapterCommand::postdo(void)
{
    std::shared_ptr<gnomonAbstractDynamicForm> output = ((gnomonAbstractLStringAdapter *) this->action)->output();

    if ((!output)||(output->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonLStringAdapterCommand::undo()
{
    ((gnomonAbstractLStringAdapter *) this->action)->setInput(nullptr);
    this->action->refreshParameters();
}

void gnomonLStringAdapterCommand::setInput(std::shared_ptr<gnomonLStringSeries> input)
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
    ((gnomonAbstractLStringAdapter *) this->action)->setInput(d->input);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonLStringSeries> gnomonLStringAdapterCommand::input()
{
    return d->input;
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonLStringAdapterCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringAdapterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringAdapterCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonLStringAdapterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonLString"));
    return input_types;
}

void gnomonLStringAdapterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonLStringSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonLStringAdapterCommand::deserializeResults(QJsonObject &serialization) {
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonLStringAdapterCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

void gnomonLStringAdapterCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->output = nullptr;
    d->input = nullptr;
}
//
// gnomonLStringAdapterCommand.cpp ends here
