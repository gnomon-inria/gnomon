#include "gnomonImagesSerieFilterCommand.h"

#include <dtkScript>

#include <dtkImagingCore>

class gnomonImagesSerieFilterCommandPrivate
{
public:
    QMap<QString, QVariant> parameters;

public:
    dtkImage* image = nullptr;
};

gnomonImagesSerieFilterCommand::gnomonImagesSerieFilterCommand(const QString& key) : d(new gnomonImagesSerieFilterCommandPrivate)
{
    QString command = "import gnomonImagesSerieFilter";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::imagesSerieFilter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImagesSerieFilterCommand::~gnomonImagesSerieFilterCommand(void)
{
    delete d;
}

void gnomonImagesSerieFilterCommand::redo(void)
{
    Q_ASSERT(this->action);
    this->action->setImage(d->image);

    gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->run();
}

void gnomonImagesSerieFilterCommand::undo(void)
{
    this->action->setImage(nullptr);
}

void gnomonImagesSerieFilterCommand::setImage(dtkImage *image)
{
    d->image = image;
}

QMap<QString, gnomonParameter*> gnomonImagesSerieFilterCommand::parameters(void) const
{
    return this->action->parameters();
}

double gnomonImagesSerieFilterCommand::time(void)
{
    return this->action->time();
}

dtkImage *gnomonImagesSerieFilterCommand::at(double t)
{
    return this->action->at(t);
}

dtkImage *gnomonImagesSerieFilterCommand::next(void)
{
    return this->action->next();
}
