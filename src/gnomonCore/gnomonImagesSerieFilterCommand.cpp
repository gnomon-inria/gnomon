#include "gnomonImagesSerieFilterCommand.h"

#include <dtkScript>

#include <dtkImagingCore>

class gnomonImagesSerieFilterCommandPrivate
{
public:
    dtkImage* image;
};

gnomonImagesSerieFilterCommand::gnomonImagesSerieFilterCommand(const QString& key) : d(new gnomonImagesSerieFilterCommandPrivate)
{
    QString command = "import " + key;

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
    gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->run();
}

void gnomonImagesSerieFilterCommand::undo(void)
{
    gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->setImage(nullptr);
}

void gnomonImagesSerieFilterCommand::setImage(dtkImage* image)
{
    std::cout << "setting image..." << std::endl;
    std::cout << image << std::endl;
    d->image = image;
}

dtkImage *gnomonImagesSerieFilterCommand::at(double t)
{
    return gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->at(t);
}

dtkImage *gnomonImagesSerieFilterCommand::next(void)
{
    return gnomonAbstractCommand<gnomonAbstractImagesSerieFilter>::action->next();
}
