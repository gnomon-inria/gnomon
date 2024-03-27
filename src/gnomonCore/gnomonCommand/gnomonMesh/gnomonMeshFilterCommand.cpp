#include "gnomonMeshFilterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshFilter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonMeshFilterCommandPrivate
{
public:
    std::shared_ptr<gnomonMeshSeries> input = nullptr;
    std::shared_ptr<gnomonCellImageSeries> cellImage = nullptr;
    std::shared_ptr<gnomonMeshSeries> output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshFilterCommand::gnomonMeshFilterCommand() : d(new gnomonMeshFilterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonMeshFilterCommand)

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
    this->action->is_async = true;
}

void gnomonMeshFilterCommand::postdo(void)
{
    std::shared_ptr<gnomonMeshSeries> mesh = ((gnomonAbstractMeshFilter *) this->action)->output();

    if ((!mesh)||(mesh->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = mesh;
    }
}

void gnomonMeshFilterCommand::undo()
{
    ((gnomonAbstractMeshFilter *) this->action)->setInput(nullptr);
    ((gnomonAbstractMeshFilter *) this->action)->setCellImage(nullptr);
    this->action->refreshParameters();
}

void gnomonMeshFilterCommand::setInput(std::shared_ptr<gnomonMeshSeries> input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractMeshFilter *) this->action)->setInput(d->input);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonMeshSeries> gnomonMeshFilterCommand::input()
{
    return d->input;
}

void gnomonMeshFilterCommand::setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage)
{
    if ((!cellImage)||(cellImage->times().empty())) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractMeshFilter *) this->action)->setCellImage(d->cellImage);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonCellImageSeries> gnomonMeshFilterCommand::cellImage()
{
    return d->cellImage;
}

std::shared_ptr<gnomonMeshSeries> gnomonMeshFilterCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonMeshFilterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["input"] = this->input();
    inputs["cellImage"] = this->cellImage();
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonMeshFilterCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonMeshFilterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonMesh"));
    input_types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonMeshFilterCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonMesh"));
    return output_types;
}

void gnomonMeshFilterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonMeshSeries>(form));
    } else if (name == "cellImage") {
        this->setCellImage(std::dynamic_pointer_cast<gnomonCellImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonMeshFilterCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonMeshSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonMeshFilterCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

//
// gnomonMeshFilterCommand.cpp ends here
