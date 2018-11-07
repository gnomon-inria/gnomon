#include "gnomonMultiChannelImageReaderCommand.h"

#include "gnomonMultiChannelImage.h"

#include <dtkScript>

class gnomonMultiChannelImageReaderCommandPrivate
{
public:
    QString path;
};

gnomonMultiChannelImageReaderCommand::gnomonMultiChannelImageReaderCommand(const QString& key) : d(new gnomonMultiChannelImageReaderCommandPrivate)
{
    QString command = "import " + key;

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    gnomonAbstractCommand<gnomonAbstractMultiChannelImageReader>::action = gnomonCore::multiChannelImageReader::pluginFactory().create(key);

    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractMultiChannelImageReader>::action);
}

gnomonMultiChannelImageReaderCommand::~gnomonMultiChannelImageReaderCommand()
{
    delete d;
}

void gnomonMultiChannelImageReaderCommand::redo(void)
{
    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractMultiChannelImageReader>::action);
    gnomonAbstractCommand<gnomonAbstractMultiChannelImageReader>::action->setPath(d->path);
    gnomonAbstractCommand<gnomonAbstractMultiChannelImageReader>::action->run();
}

void gnomonMultiChannelImageReaderCommand::undo(void)
{
    gnomonAbstractCommand<gnomonAbstractMultiChannelImageReader>::action->setPath("");
}

void gnomonMultiChannelImageReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonMultiChannelImage *gnomonMultiChannelImageReaderCommand::image(void)
{
    return gnomonAbstractCommand<gnomonAbstractMultiChannelImageReader>::action->image();
}

QStringList gnomonMultiChannelImageReaderCommand::types(void)
{
    return gnomonAbstractCommand<gnomonAbstractMultiChannelImageReader>::action->types();
}
