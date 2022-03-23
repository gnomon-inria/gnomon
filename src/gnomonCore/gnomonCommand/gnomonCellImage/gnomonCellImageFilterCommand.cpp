#include "gnomonCellImageFilterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageFilter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <dtkLog>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonCellImageFilterCommandPrivate
{
public:
    gnomonCellImageSeries* input = nullptr;
    gnomonCellImageSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageFilterCommand::gnomonCellImageFilterCommand() : d(new gnomonCellImageFilterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellImageFilter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellImageFilter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellImageFilterCommand::~gnomonCellImageFilterCommand()
{
    delete d;
}

void gnomonCellImageFilterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellImageFilter::pluginFactory().create(algo_name);
}

void gnomonCellImageFilterCommand::predo(void)
{

}

void gnomonCellImageFilterCommand::postdo(void)
{
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageFilter *) this->action)->output();

    if ((!cellImage)||(cellImage->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = cellImage;
    }
}

void gnomonCellImageFilterCommand::undo()
{
    ((gnomonAbstractCellImageFilter *) this->action)->setInput(nullptr);
}

void gnomonCellImageFilterCommand::setInput(gnomonCellImageSeries *input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractCellImageFilter *) this->action)->setInput(d->input);
    }
}

gnomonCellImageSeries *gnomonCellImageFilterCommand::input()
{
    return d->input;
}

gnomonCellImageSeries *gnomonCellImageFilterCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageFilterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageFilterCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonCellImageFilterCommand::isEmpty()
{
    return availablePlugins().empty();
}

gnomonAbstractCommand::orderedMap gnomonCellImageFilterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonCellImage"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonCellImageFilterCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonCellImage"));
    return types;
}

void gnomonCellImageFilterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonCellImageSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

QStringList gnomonCellImageFilterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

void gnomonCellImageFilterCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = new gnomonCellImageSeries();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonCellImageFilterCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

//
// gnomonCellImageFilterCommand.cpp ends here
