#include "gnomonImagesSerieReaderCommand.h"

#include <dtkScript>

class gnomonImagesSerieReaderCommandPrivate
{
public:
    QString path;
};

gnomonImagesSerieReaderCommand::gnomonImagesSerieReaderCommand(const QString& key) : d(new gnomonImagesSerieReaderCommandPrivate)
{
    QString command = "import " + key;

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    gnomonAbstractCommand<gnomonAbstractImagesSerieReader>::action = gnomonCore::imagesSerieReader::pluginFactory().create(key);

    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractImagesSerieReader>::action);
}

gnomonImagesSerieReaderCommand::~gnomonImagesSerieReaderCommand()
{
    delete d;
}

void gnomonImagesSerieReaderCommand::redo(void)
{
    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractImagesSerieReader>::action);
    gnomonAbstractCommand<gnomonAbstractImagesSerieReader>::action->setPath(d->path);
    gnomonAbstractCommand<gnomonAbstractImagesSerieReader>::action->run();
}

void gnomonImagesSerieReaderCommand::undo(void)
{
    gnomonAbstractCommand<gnomonAbstractImagesSerieReader>::action->setPath("");
}

void gnomonImagesSerieReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

double gnomonImagesSerieReaderCommand::time(void)
{
    return gnomonAbstractCommand<gnomonAbstractImagesSerieReader>::action->time();
}

dtkImage *gnomonImagesSerieReaderCommand::at(double t, const QString& channel)
{
    return gnomonAbstractCommand<gnomonAbstractImagesSerieReader>::action->at(t, channel);
}

dtkImage *gnomonImagesSerieReaderCommand::next(void)
{
    return gnomonAbstractCommand<gnomonAbstractImagesSerieReader>::action->next();
}

QStringList gnomonImagesSerieReaderCommand::channels(void)
{
    return gnomonAbstractCommand<gnomonAbstractImagesSerieReader>::action->channels();
}
