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

#include "gnomonImageFusionCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageFusionCommandPrivate
{
public:
    QVector<gnomonImageSeries *> images_series;
    QVector<std::vector<gnomonLandmark>> landmarks;

    gnomonImageSeries* output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageFusionCommand::gnomonImageFusionCommand() : d(new gnomonImageFusionCommandPrivate)
{
    this->factory_name = "imageFusion";
    loadPluginGroup(this->factoryName());

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

void gnomonImageFusionCommand::redo()
{
    Q_ASSERT(this->action);

//    for(auto& images_serie : d->images_series) {
//        ((gnomonAbstractImageFusion *) this->action)->addImage(images_serie);
//    };

    for(auto& landmarks : d->landmarks) {
        ((gnomonAbstractImageFusion *) this->action)->addLandmarks(landmarks);
    }

    this->action->run();

    gnomonImageSeries *image = ((gnomonAbstractImageFusion *) this->action)->output();
    if ((!image)||(image->times().empty())||(((gnomonImage *)image->current())->channels().empty())) {
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

void gnomonImageFusionCommand::addImage(gnomonImageSeries *image_series)
{
    d->images_series.push_back(image_series);

    ((gnomonAbstractImageFusion *) this->action)->removeImages();
    for(auto& image_series : d->images_series) {
        ((gnomonAbstractImageFusion *) this->action)->addImage(image_series);
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

gnomonImageSeries *gnomonImageFusionCommand::output() const
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageFusionCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    int input_count = 1;
    for(auto& image_series : d->images_series) {
        QString input_name = "image" + QString::number(input_count);
        inputs[input_name] = image_series;
        input_count++;
    }
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageFusionCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonImageFusionCommand::isEmpty()
{
    loadPluginGroup("imageFusion");
    return gnomonCore::imageFusion::pluginFactory().keys().empty();
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

void gnomonImageFusionCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    // TODO: come back later to see if correct
    dtkWarn()<<Q_FUNC_INFO<<"Implementation uncertain !!!";
    if (name == "image") {
        this->addImage(dynamic_cast<gnomonImageSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonImageFusionCommand.cpp ends here
