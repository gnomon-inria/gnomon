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
    std::shared_ptr<gnomonCellImageSeries> input = nullptr;
    std::shared_ptr<gnomonCellImageSeries> output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageFilterCommand::gnomonCellImageFilterCommand() : d(new gnomonCellImageFilterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonCellImageFilterCommand)

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
    std::shared_ptr<gnomonCellImageSeries> cellImage = ((gnomonAbstractCellImageFilter *) this->action)->output();

    if ((!cellImage)||(cellImage->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = cellImage;
    }
}

void gnomonCellImageFilterCommand::undo()
{
    ((gnomonAbstractCellImageFilter *) this->action)->setInput(nullptr);
    this->action->refreshParameters();
}

void gnomonCellImageFilterCommand::setInput(std::shared_ptr<gnomonCellImageSeries> input)
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
    ((gnomonAbstractCellImageFilter *) this->action)->setInput(d->input);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonCellImageSeries> gnomonCellImageFilterCommand::input()
{
    return d->input;
}

std::shared_ptr<gnomonCellImageSeries> gnomonCellImageFilterCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageFilterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageFilterCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
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

void gnomonCellImageFilterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "input") {
        this->setInput(std::dynamic_pointer_cast<gnomonCellImageSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonCellImageFilterCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonCellImageSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonCellImageFilterCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

void gnomonCellImageFilterCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->output = nullptr;
    d->input = nullptr;
}

//
// gnomonCellImageFilterCommand.cpp ends here
