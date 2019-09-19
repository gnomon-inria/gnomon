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
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageFusionCommand::gnomonImageFusionCommand(const QString& key) : d(new gnomonImageFusionCommandPrivate)
{
    QString command = "import gnomonImageFusion";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

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
}

void gnomonImageFusionCommand::undo(void)
{
    d->images_series.clear();
    ((gnomonAbstractImageFusion *) this->action)->removeLandmarks();
}


void gnomonImageFusionCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

void gnomonImageFusionCommand::addImage(gnomonImageSeries *image_series)
{
    d->images_series.push_back(image_series);

    ((gnomonAbstractImageRegistration *) this->action)->removeImages();
    for(auto& images_serie : d->images_series) {
        ((gnomonAbstractImageRegistration *) this->action)->addImage(images_serie);
    };
}

void gnomonImageFusionCommand::removeImages(void)
{
    d->images_series.clear();
}

void gnomonImageFusionCommand::addLandmarks(const std::vector<gnomonLandmark>& landmarks)
{
    d->landmarks.append(landmarks);
}

void gnomonImageFusionCommand::removeLandmarks(void)
{
    d->landmarks.clear();
}

QMap<QString, gnomonCoreParameter *> gnomonImageFusionCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImageSeries *gnomonImageFusionCommand::output(void)
{
    return ((gnomonAbstractImageFusion *) this->action)->output();
}

//
// gnomonImageFusionCommand.cpp ends here
