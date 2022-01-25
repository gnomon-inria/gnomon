// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonTreeFromLStringCommand.h"

#include <dtkScript>

class gnomonTreeFromLStringCommandPrivate
{
public:
    gnomonLStringSeries *input = nullptr;
    gnomonTreeSeries *output = nullptr;
};

gnomonTreeFromLStringCommand::gnomonTreeFromLStringCommand() : d(new gnomonTreeFromLStringCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

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
    gnomonTreeSeries *tree = ((gnomonAbstractTreeFromLString *) this->action)->output();

    if ((!tree)||(tree->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = tree;
    }
}

void gnomonTreeFromLStringCommand::undo()
{
    ((gnomonAbstractTreeFromLString *) this->action)->setInput(nullptr);
}

//void gnomonTreeFromLStringCommand::setLSystem(const QString& lsystem)
//{
//    d->lsystem = lsystem;
//}

void gnomonTreeFromLStringCommand::setInput(gnomonLStringSeries *input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractTreeFromLString *) this->action)->setInput(d->input);
    }
}

gnomonLStringSeries *gnomonTreeFromLStringCommand::input()
{
    return d->input;
}

gnomonTreeSeries *gnomonTreeFromLStringCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeFromLStringCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeFromLStringCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonTreeFromLStringCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonTreeFromLStringCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
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

void gnomonTreeFromLStringCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonLStringSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonTreeFromLStringCommand.cpp ends here
