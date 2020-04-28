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

gnomonImageFusionCommand::gnomonImageFusionCommand(const QString& key) : d(new gnomonImageFusionCommandPrivate)
{
    this->factory_name = "imageFusion";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::imageFusion::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImageFusionCommand::~gnomonImageFusionCommand(void)
{
    delete d;
}

void gnomonImageFusionCommand::redo(void)
{
    Q_ASSERT(this->action);

//    for(auto& images_serie : d->images_series) {
//        ((gnomonAbstractImageFusion *) this->action)->addImage(images_serie);
//    };

    for(auto& landmarks : d->landmarks) {
        ((gnomonAbstractImageFusion *) this->action)->addLandmarks(landmarks);
    };

    this->action->run();

    gnomonImageSeries *image = ((gnomonAbstractImageFusion *) this->action)->output();
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        d->output = nullptr;
    } else {
        d->output = image;
    }
}

void gnomonImageFusionCommand::undo(void)
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
    };
}

void gnomonImageFusionCommand::addLandmarks(const std::vector<gnomonLandmark>& landmarks)
{
    d->landmarks.append(landmarks);
}

void gnomonImageFusionCommand::removeLandmarks(void)
{
    d->landmarks.clear();
}

void gnomonImageFusionCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonImageFusionCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImageSeries *gnomonImageFusionCommand::output(void)
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageFusionCommand::inputs(void)
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

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageFusionCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonImageFusionCommand::isEmpty(void)
{
    loadPluginGroup("imageFusion");
    return gnomonCore::imageFusion::pluginFactory().keys().size() == 0;
}

//
// gnomonImageFusionCommand.cpp ends here
