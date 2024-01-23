#include "gnomonTreeFromLStringCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeFromLString.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonTreeFromLStringCommandPrivate
{
public:
    std::shared_ptr<gnomonLStringSeries> input = nullptr;
    std::shared_ptr<gnomonTreeSeries> output = nullptr;
};

gnomonTreeFromLStringCommand::gnomonTreeFromLStringCommand() : d(new gnomonTreeFromLStringCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonTreeFromLStringCommand)

    QStringList keys = gnomonCore::treeFromLString::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeFromLString::pluginFactory().create(this->algorithm_name);
    }
}

gnomonTreeFromLStringCommand::~gnomonTreeFromLStringCommand()
{
    delete d;
}

void gnomonTreeFromLStringCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::treeFromLString::pluginFactory().create(algo_name);
}


void gnomonTreeFromLStringCommand::predo(void)
{

}

void gnomonTreeFromLStringCommand::postdo(void)
{
    std::shared_ptr<gnomonTreeSeries> tree = ((gnomonAbstractTreeFromLString *) this->action)->output();

    if ((!tree)||(tree->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = tree;
    }
}

void gnomonTreeFromLStringCommand::undo()
{
    ((gnomonAbstractTreeFromLString *) this->action)->setInput(nullptr);
    this->action->refreshParameters();
}

//void gnomonTreeFromLStringCommand::setLSystem(const QString& lsystem)
//{
//    d->lsystem = lsystem;
//}

void gnomonTreeFromLStringCommand::setInput(std::shared_ptr<gnomonLStringSeries> input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractTreeFromLString *) this->action)->setInput(d->input);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonLStringSeries> gnomonTreeFromLStringCommand::input()
{
    return d->input;
}

std::shared_ptr<gnomonTreeSeries> gnomonTreeFromLStringCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonTreeFromLStringCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonTreeFromLStringCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonTreeFromLStringCommand::inputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("input", "gnomonLString"));
    return types;
}

gnomonAbstractCommand::orderedMap gnomonTreeFromLStringCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonTree"));
    return types;
}

void gnomonTreeFromLStringCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonLStringSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonTreeFromLStringCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonTreeSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonTreeFromLStringCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

void gnomonTreeFromLStringCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->input = nullptr;
    d->output = nullptr;
}

//
// gnomonTreeFromLStringCommand.cpp ends here
