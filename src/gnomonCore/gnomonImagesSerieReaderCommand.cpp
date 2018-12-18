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
    ((gnomonAbstractImagesSerieReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonImagesSerieReaderCommand::undo(void)
{
    ((gnomonAbstractImagesSerieReader *) this->action)->setPath("");
}

void gnomonImagesSerieReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonImagesSerie *gnomonImagesSerieReaderCommand::imagesSerie()
{
    return ((gnomonAbstractImagesSerieReader *) this->action)->imagesSerie();
}