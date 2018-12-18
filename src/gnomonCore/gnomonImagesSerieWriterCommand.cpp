#include "gnomonImagesSerieWriterCommand.h"

#include <dtkScript>

class gnomonImagesSerieWriterCommandPrivate
{
public:
    QString path;
    gnomonImagesSerie* imagesSerie = nullptr;
};

gnomonImagesSerieWriterCommand::gnomonImagesSerieWriterCommand(const QString& key) : d(new gnomonImagesSerieWriterCommandPrivate)
{
    QString command = "import gnomonImagesSerieWriter";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::imagesSerieWriter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImagesSerieWriterCommand::~gnomonImagesSerieWriterCommand()
{
    delete d;
}

void gnomonImagesSerieWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractImagesSerieWriter *) this->action)->setPath(d->path);
    ((gnomonAbstractImagesSerieWriter *) this->action)->setImagesSerie(d->imagesSerie);
    this->action->run();
}

void gnomonImagesSerieWriterCommand::undo(void)
{
    ((gnomonAbstractImagesSerieWriter *) this->action)->setPath("");
}

void gnomonImagesSerieWriterCommand::setPath(const QString& path)
{
    d->path = path;
}

void gnomonImagesSerieWriterCommand::setImagesSerie(gnomonImagesSerie *imagesSerie)
{
    d->imagesSerie = imagesSerie;
}