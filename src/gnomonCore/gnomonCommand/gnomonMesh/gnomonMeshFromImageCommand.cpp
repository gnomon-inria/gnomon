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

#include "gnomonMeshFromImageCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonMeshFromImageCommandPrivate
{
public:
    gnomonImageSeries *input = nullptr;
    gnomonMeshSeries *output = nullptr;
};

gnomonMeshFromImageCommand::gnomonMeshFromImageCommand() : d(new gnomonMeshFromImageCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::meshFromImage::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::meshFromImage::pluginFactory().create(this->algorithm_name);
    }
}

gnomonMeshFromImageCommand::~gnomonMeshFromImageCommand()
{
    delete d;
}

void gnomonMeshFromImageCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::meshFromImage::pluginFactory().create(algo_name);
}

void gnomonMeshFromImageCommand::predo(void)
{

}

void gnomonMeshFromImageCommand::postdo(void)
{
    gnomonMeshSeries *mesh = ((gnomonAbstractMeshFromImage *) this->action)->output();

    if ((!mesh)||(mesh->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = mesh;
    }
}

void gnomonMeshFromImageCommand::undo()
{
    ((gnomonAbstractMeshFromImage *) this->action)->setInput(nullptr);
}

void gnomonMeshFromImageCommand::setInput(gnomonImageSeries *input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractMeshFromImage *) this->action)->setInput(d->input);
    }
}

gnomonImageSeries *gnomonMeshFromImageCommand::input()
{
    return d->input;
}

gnomonMeshSeries *gnomonMeshFromImageCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshFromImageCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonMeshFromImageCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonMeshFromImageCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonMeshFromImageCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonMeshFromImageCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonImage"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonMeshFromImageCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonMesh"));
    return output_types;
}

void gnomonMeshFromImageCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonImageSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonMeshFromImageCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = new gnomonMeshSeries();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonMeshFromImageCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

//
// gnomonMeshFromImageCommand.cpp ends here
