#include "gnomonImagesSerieFilterCommand.h"

#include <dtkScript>

#include <dtkImagingCore>

class gnomonImagesSerieFilterCommandPrivate
{
public:
    gnomonImagesSerie* input = nullptr;
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
    this->action->setInput(d->input);

    this->action->run();
}

void gnomonImagesSerieFilterCommand::undo(void)
{
    this->action->setInput(nullptr);
}

void gnomonImagesSerieFilterCommand::setInput(gnomonImagesSerie *input)
{
    d->input = input;
}

void gnomonImagesSerieFilterCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonImagesSerieFilterCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImagesSerie *gnomonImagesSerieFilterCommand::input(void)
{
    return this->action->input();
}

gnomonImagesSerie *gnomonImagesSerieFilterCommand::output(void)
{
    return this->action->output();
}
