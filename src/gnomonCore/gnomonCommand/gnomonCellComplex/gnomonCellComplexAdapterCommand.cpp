#include "gnomonCellComplexAdapterCommand.h"
#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexAdapter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonCellComplexAdapterCommandPrivate
{
public:
    gnomonCellComplexSeries* input = nullptr;
    gnomonAbstractDynamicForm* output = nullptr;
};

gnomonCellComplexAdapterCommand::gnomonCellComplexAdapterCommand() : d(new gnomonCellComplexAdapterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

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
    gnomonAbstractDynamicForm *output = ((gnomonAbstractCellComplexAdapter *) this->action)->output();

    if ((!output)||(output->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonCellComplexAdapterCommand::undo()
{
    ((gnomonAbstractCellComplexAdapter *) this->action)->setInput(nullptr);
}

void gnomonCellComplexAdapterCommand::setInput(gnomonCellComplexSeries *input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellComplexAdapter *) this->action)->setInput(d->input);
    }
}

gnomonCellComplexSeries *gnomonCellComplexAdapterCommand::input()
{
    return d->input;
}

gnomonAbstractDynamicForm *gnomonCellComplexAdapterCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexAdapterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexAdapterCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonCellComplexAdapterCommand::isEmpty()
{
    return availablePlugins().empty();
}

gnomonAbstractCommand::orderedMap gnomonCellComplexAdapterCommand::inputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("input", "gnomonCellComplex"));
    return types;
}

void gnomonCellComplexAdapterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonCellComplexSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

QStringList gnomonCellComplexAdapterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
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

//
// gnomonCellComplexAdapterCommand.cpp ends here
