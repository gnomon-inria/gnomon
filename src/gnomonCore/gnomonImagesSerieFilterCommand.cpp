#include "gnomonImagesSerieFilterCommand.h"

#include <dtkScript>

#include <dtkImagingCore>

class gnomonImagesSerieFilterCommandPrivate
{
public:
    QMap<QString, QVariant> parameters;

public:
    dtkImage* image;
};

gnomonImagesSerieFilterCommand::gnomonImagesSerieFilterCommand(const QString& key) : d(new gnomonImagesSerieFilterCommandPrivate)
{
    QString command = "import gnomonImagesSerieFilter";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action = gnomonCore::imagesSerieFilter::pluginFactory().create(key);

    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action);
}

gnomonImagesSerieFilterCommand::~gnomonImagesSerieFilterCommand()
{
    delete d;
}

void gnomonImagesSerieFilterCommand::redo(void)
{
    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action);
    gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->setImage(d->image);

    QMap<QString, QVariant>::const_iterator i = d->parameters.constBegin();
    while (i != d->parameters.constEnd()) {
        gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->setParameter(i.key(), i.value());
        ++i;
    }

    gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->run();
}

void gnomonImagesSerieFilterCommand::undo(void)
{
    gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->setImage(nullptr);
}

void gnomonImagesSerieFilterCommand::setImage(dtkImage* image)
{
    d->image = image;
}

void gnomonImagesSerieFilterCommand::setParameter(const QString& param_name, const QVariant& param_value)
{
    d->parameters[param_name] = param_value;
}

QMap<QString, QVariant> gnomonImagesSerieFilterCommand::parameters(void) const
{
    return gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->parameters();
}

double gnomonImagesSerieFilterCommand::time(void)
{
    return gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->time();
}

dtkImage *gnomonImagesSerieFilterCommand::at(double t)
{
    return gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->at(t);
}

dtkImage *gnomonImagesSerieFilterCommand::next(void)
{
    return gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->next();
}
