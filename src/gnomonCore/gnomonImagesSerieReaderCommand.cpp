#include "gnomonImagesSerieReaderCommand.h"

#include <dtkScript>

class gnomonImagesSerieReaderCommandPrivate
{
public:
    QString path;
};

gnomonImagesSerieReaderCommand::gnomonImagesSerieReaderCommand(const QString& key) : d(new gnomonImagesSerieReaderCommandPrivate)
{
    QString command = "import gnomonImagesSerieReader";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::imagesSerieReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImagesSerieReaderCommand::~gnomonImagesSerieReaderCommand()
{
    delete d;
}

void gnomonImagesSerieReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    this->action->setPath(d->path);
    this->action->run();
}

void gnomonImagesSerieReaderCommand::undo(void)
{
    this->action->setPath("");
}

void gnomonImagesSerieReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

double gnomonImagesSerieReaderCommand::time(void)
{
    return this->action->time();
}

dtkImage *gnomonImagesSerieReaderCommand::at(double t, const QString& channel)
{
    return this->action->at(t, channel);
}

dtkImage *gnomonImagesSerieReaderCommand::next(void)
{
    return this->action->next();
}

QStringList gnomonImagesSerieReaderCommand::channels(void)
{
    return this->action->channels();
}
