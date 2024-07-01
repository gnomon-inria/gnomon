#include "gnomonImageFusionCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageFusion.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageFusionCommandPrivate
{
public:
    QVector<std::shared_ptr<gnomonImageSeries> > images_series;
    QVector<std::vector<gnomonLandmark>> landmarks;
    QMap<QString, int> image_indices;

    std::shared_ptr<gnomonImageSeries> output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageFusionCommand::gnomonImageFusionCommand() : d(new gnomonImageFusionCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonImageFusionCommand)

    QStringList keys = gnomonCore::imageFusion::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::imageFusion::pluginFactory().create(this->algorithm_name);
    }
}

gnomonImageFusionCommand::~gnomonImageFusionCommand()
{
    delete d;
}

void gnomonImageFusionCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::imageFusion::pluginFactory().create(algo_name);
}

void gnomonImageFusionCommand::predo(void)
{
//    for(auto& images_serie : d->images_series) {
//        ((gnomonAbstractImageFusion *) this->action)->addImage(images_serie);
//    };

    for(auto& landmarks : d->landmarks) {
        ((gnomonAbstractImageFusion *) this->action)->addLandmarks(landmarks);
    }
}

void gnomonImageFusionCommand::postdo(void)
{
    std::shared_ptr<gnomonImageSeries> image = ((gnomonAbstractImageFusion *) this->action)->output();

    if ((!image)||(image->times().empty())||(image->current()->channels().empty())) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonImageFusionCommand::undo()
{
    d->images_series.clear();
    ((gnomonAbstractImageFusion *) this->action)->removeLandmarks();
}

void gnomonImageFusionCommand::addImage(std::shared_ptr<gnomonImageSeries> image_series)
{
    int index = d->images_series.size();
    QString input_name = "image" + QString::number(index);
    d->image_indices[input_name] = index;
    d->images_series.push_back(image_series);

    reloadImages();
}

void gnomonImageFusionCommand::changeImage(const QString& name, std::shared_ptr<gnomonImageSeries> image_series) {
    const auto& current_inputs = this->inputs();
    if (current_inputs.contains(name)) {
        d->images_series[d->image_indices[name]] = image_series;
    }
    reloadImages();
}

void gnomonImageFusionCommand::reloadImages() {
    ((gnomonAbstractImageFusion *) action)->removeImages();
    for(auto& image_series : d->images_series) {
        ((gnomonAbstractImageFusion *) action)->addImage(image_series);
    }
}

void gnomonImageFusionCommand::addLandmarks(const std::vector<gnomonLandmark>& landmarks)
{
    d->landmarks.append(landmarks);
}

void gnomonImageFusionCommand::removeLandmarks()
{
    d->landmarks.clear();
}

std::shared_ptr<gnomonImageSeries> gnomonImageFusionCommand::output() const
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonImageFusionCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    int i = 0;
    for(auto& image_series : d->images_series) {
        QString input_name = d->image_indices.key(i);
        inputs[input_name] = image_series;
        i++;
    }
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonImageFusionCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonImageFusionCommand::inputTypes() {
    orderedMap input_types;
    int input_count = 0;
    for(auto& image_series : d->images_series) {
        QString input_name = "image" + QString::number(input_count);
        input_types.emplace_back(std::make_pair(input_name, "gnomonImage"));
        input_count++;
    }
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonImageFusionCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonImage"));
    return types;
}

void gnomonImageFusionCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    // TODO: come back later to see if correct
    dtkWarn()<<Q_FUNC_INFO<<"Implementation uncertain !!!";
    std::shared_ptr<gnomonImageSeries> form_cast = std::dynamic_pointer_cast<gnomonImageSeries>(form);
    if (form_cast && this->inputs().contains(name)) {
        this->changeImage(name, form_cast);
    } else if (form_cast) {
        this->addImage(form_cast);
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonImageFusionCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonImageSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonImageFusionCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

void gnomonImageFusionCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->output = nullptr;
    d->images_series.clear();
    d->image_indices.clear();
    d->landmarks.clear();
}

//
// gnomonImageFusionCommand.cpp ends here
