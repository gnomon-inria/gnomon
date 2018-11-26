#include "gnomonImagesFusionCommand.h"

#include <dtkScript>

class gnomonImagesFusionCommandPrivate
{
public:
    QVector<dtkImage *> images;
    QVector<std::vector<gnomonLandmark>> landmarks;
};

gnomonImagesFusionCommand::gnomonImagesFusionCommand(const QString& key) : d(new gnomonImagesFusionCommandPrivate)
{
    QString command = "import gnomonImagesFusion";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::imagesFusion::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImagesFusionCommand::~gnomonImagesFusionCommand(void)
{
    delete d;
}

void gnomonImagesFusionCommand::redo(void)
{
    Q_ASSERT(this->action);

    for(auto& image : d->images) {
        this->action->addImage(image);
    };

    for(auto& landmarks : d->landmarks) {
        this->action->addLandmarks(landmarks);
    };

    this->action->run();
}

void gnomonImagesFusionCommand::undo(void)
{
    this->action->removeImages();
    this->action->removeLandmarks();
}


void gnomonImagesFusionCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

void gnomonImagesFusionCommand::addImage(dtkImage *image)
{
    d->images.push_back(image);
}

void gnomonImagesFusionCommand::removeImages(void)
{
    d->images.clear();
}

void gnomonImagesFusionCommand::addLandmarks(const std::vector<gnomonLandmark>& landmarks)
{
    d->landmarks.append(landmarks);
}

void gnomonImagesFusionCommand::removeLandmarks(void)
{
    d->landmarks.clear();
}

QMap<QString, gnomonCoreParameter *> gnomonImagesFusionCommand::parameters(void) const
{
    return this->action->parameters();
}

dtkImage *gnomonImagesFusionCommand::output(void)
{
    return this->action->output();
}
