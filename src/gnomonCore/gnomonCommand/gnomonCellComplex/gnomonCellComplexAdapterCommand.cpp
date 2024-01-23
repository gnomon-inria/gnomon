#include "gnomonCellComplexAdapterCommand.h"
#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexAdapter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonCellComplexAdapterCommandPrivate
{
public:
    std::shared_ptr<gnomonCellComplexSeries> input;
    std::shared_ptr<gnomonAbstractDynamicForm> output;
};

gnomonCellComplexAdapterCommand::gnomonCellComplexAdapterCommand() : d(new gnomonCellComplexAdapterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonCellComplexAdapterCommand)

    QStringList keys = gnomonCore::cellComplexAdapter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellComplexAdapter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellComplexAdapterCommand::~gnomonCellComplexAdapterCommand()
{
    delete d;
}

void gnomonCellComplexAdapterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellComplexAdapter::pluginFactory().create(algo_name);
}

void gnomonCellComplexAdapterCommand::predo(void) {}
void gnomonCellComplexAdapterCommand::postdo(void)
{
    std::shared_ptr<gnomonAbstractDynamicForm> output = ((gnomonAbstractCellComplexAdapter *) this->action)->output();

    if ((!output)||(output->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonCellComplexAdapterCommand::undo()
{
    ((gnomonAbstractCellComplexAdapter *) this->action)->setInput(nullptr);
    this->action->refreshParameters();
}

void gnomonCellComplexAdapterCommand::setInput(std::shared_ptr<gnomonCellComplexSeries> input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractCellComplexAdapter *) this->action)->setInput(d->input);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonCellComplexSeries> gnomonCellComplexAdapterCommand::input()
{
    return d->input;
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonCellComplexAdapterCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellComplexAdapterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellComplexAdapterCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonCellComplexAdapterCommand::inputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("input", "gnomonCellComplex"));
    return types;
}

void gnomonCellComplexAdapterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonCellComplexSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonCellComplexAdapterCommand::deserializeResults(QJsonObject &serialization) {
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonCellComplexAdapterCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

void gnomonCellComplexAdapterCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->output = nullptr;
    d->input = nullptr;
}

//
// gnomonCellComplexAdapterCommand.cpp ends here
