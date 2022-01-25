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

#include "gnomonMeshFilterCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonMeshFilterCommandPrivate
{
public:
    gnomonMeshSeries* input = nullptr;
    gnomonMeshSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshFilterCommand::gnomonMeshFilterCommand() : d(new gnomonMeshFilterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::meshFilter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::meshFilter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonMeshFilterCommand::~gnomonMeshFilterCommand()
{
    delete d;
}

void gnomonMeshFilterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::meshFilter::pluginFactory().create(algo_name);
}

void gnomonMeshFilterCommand::predo(void)
{

}

void gnomonMeshFilterCommand::postdo(void)
{
    gnomonMeshSeries *mesh = ((gnomonAbstractMeshFilter *) this->action)->output();

    if ((!mesh)||(mesh->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = mesh;
    }
}

void gnomonMeshFilterCommand::undo()
{
    ((gnomonAbstractMeshFilter *) this->action)->setInput(nullptr);
}

void gnomonMeshFilterCommand::setInput(gnomonMeshSeries *input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractMeshFilter *) this->action)->setInput(d->input);
    }
}

gnomonMeshSeries *gnomonMeshFilterCommand::input()
{
    return d->input;
}

gnomonMeshSeries *gnomonMeshFilterCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshFilterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshFilterCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonMeshFilterCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonMeshFilterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonMeshFilterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonMesh"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonMeshFilterCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonMesh"));
    return output_types;
}

void gnomonMeshFilterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonMeshSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonMeshFilterCommand.cpp ends here
