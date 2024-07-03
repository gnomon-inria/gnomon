#include "gnomonCellComplexFromCellImageCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexFromCellImage.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonCellComplexFromCellImageCommandPrivate
{
public:
    std::shared_ptr<gnomonCellImageSeries> input = nullptr;
    std::shared_ptr<gnomonCellComplexSeries> output = nullptr;
};

gnomonCellComplexFromCellImageCommand::gnomonCellComplexFromCellImageCommand() : d(new gnomonCellComplexFromCellImageCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonCellComplexFromCellImageCommand)

    QStringList keys = gnomonCore::cellComplexFromCellImage::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellComplexFromCellImage::pluginFactory().create(this->algorithm_name);
    }

}

gnomonCellComplexFromCellImageCommand::~gnomonCellComplexFromCellImageCommand()
{
    delete d;
}

void gnomonCellComplexFromCellImageCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    delete this->action;
    this->action = gnomonCore::cellComplexFromCellImage::pluginFactory().create(algo_name);
}

void gnomonCellComplexFromCellImageCommand::predo(void)
{
    this->action->is_async = true;
}

void gnomonCellComplexFromCellImageCommand::postdo(void)
{
    std::shared_ptr<gnomonCellComplexSeries> cellComplex = ((gnomonAbstractCellComplexFromCellImage *) this->action)->output();

    if ((!cellComplex)||(cellComplex->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = cellComplex;
    }
}

void gnomonCellComplexFromCellImageCommand::undo()
{
    ((gnomonAbstractCellComplexFromCellImage *) this->action)->setInput(nullptr);
    this->action->refreshParameters();
}

void gnomonCellComplexFromCellImageCommand::setInput(std::shared_ptr<gnomonCellImageSeries> input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
    }
    Q_ASSERT(this->action);
    if (d->input) {
        d->input->load();
    }
    ((gnomonAbstractCellComplexFromCellImage *) this->action)->setInput(d->input);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonCellImageSeries> gnomonCellComplexFromCellImageCommand::input()
{
    return d->input;
}

std::shared_ptr<gnomonCellComplexSeries> gnomonCellComplexFromCellImageCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellComplexFromCellImageCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellComplexFromCellImageCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonCellComplexFromCellImageCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonCellImage"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonCellComplexFromCellImageCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonCellComplex"));
    return types;
}

void gnomonCellComplexFromCellImageCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonCellImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonCellComplexFromCellImageCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonCellComplexSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonCellComplexFromCellImageCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

void gnomonCellComplexFromCellImageCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->output = nullptr;
    d->input = nullptr;
}

//
// gnomonCellComplexFromCellImageCommand.cpp ends here
