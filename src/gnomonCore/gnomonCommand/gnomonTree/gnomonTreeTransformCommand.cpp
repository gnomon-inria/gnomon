#include "gnomonTreeTransformCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeTransform.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonTreeTransformCommandPrivate
{
public:
    std::shared_ptr<gnomonTreeSeries> input = nullptr;
    std::shared_ptr<gnomonTreeSeries> output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeTransformCommand::gnomonTreeTransformCommand() : d(new gnomonTreeTransformCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeTransform::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeTransform::pluginFactory().create(this->algorithm_name);
    }
}

gnomonTreeTransformCommand::~gnomonTreeTransformCommand()
{
    delete d;
}

void gnomonTreeTransformCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::treeTransform::pluginFactory().create(algo_name);
}

void gnomonTreeTransformCommand::predo(void)
{

}

void gnomonTreeTransformCommand::postdo(void)
{
    std::shared_ptr<gnomonTreeSeries> tree = ((gnomonAbstractTreeTransform *) this->action)->output();

    if ((!tree)||(tree->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = tree;
    }
}

void gnomonTreeTransformCommand::undo()
{
    ((gnomonAbstractTreeTransform *) this->action)->setInput(nullptr);
    this->action->refreshParameters();
}

void gnomonTreeTransformCommand::setInput(std::shared_ptr<gnomonTreeSeries> input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractTreeTransform *) this->action)->setInput(d->input);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonTreeSeries> gnomonTreeTransformCommand::input()
{
    return d->input;
}

std::shared_ptr<gnomonTreeSeries> gnomonTreeTransformCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonTreeTransformCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonTreeTransformCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonTreeTransformCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonTreeTransformCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonTreeTransformCommand::inputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("input", "gnomonTree"));
    return types;
}

gnomonAbstractCommand::orderedMap gnomonTreeTransformCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonTree"));
    return types;}

void gnomonTreeTransformCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonTreeSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonTreeTransformCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonTreeSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonTreeTransformCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

//
// gnomonTreeTransformCommand.cpp ends here
