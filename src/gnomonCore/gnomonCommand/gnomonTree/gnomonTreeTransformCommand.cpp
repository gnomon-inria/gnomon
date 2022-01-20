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

#include "gnomonTreeTransformCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonTreeTransformCommandPrivate
{
public:
    gnomonTreeSeries* input = nullptr;
    gnomonTreeSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeTransformCommand::gnomonTreeTransformCommand() : d(new gnomonTreeTransformCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeFromLString::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeFromLString::pluginFactory().create(this->algorithm_name);
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

void gnomonTreeTransformCommand::redo()
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonTreeSeries *tree = ((gnomonAbstractTreeTransform *) this->action)->output();
    if ((!tree)||(tree->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = tree;
    }
}

void gnomonTreeTransformCommand::undo()
{
    ((gnomonAbstractTreeTransform *) this->action)->setInput(nullptr);
}

void gnomonTreeTransformCommand::setInput(gnomonTreeSeries *input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractTreeTransform *) this->action)->setInput(d->input);
    }
}

gnomonTreeSeries *gnomonTreeTransformCommand::input()
{
    return d->input;
}

gnomonTreeSeries *gnomonTreeTransformCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeTransformCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeTransformCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
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

void gnomonTreeTransformCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonTreeSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonTreeTransformCommand.cpp ends here
