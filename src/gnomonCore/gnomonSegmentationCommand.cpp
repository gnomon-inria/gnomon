#include "gnomonSegmentationCommand.h"

#include "gnomonCellImage.h"

#include "gnomonImagesSerie.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonSegmentationCommandPrivate
{
public:
    gnomonImagesSerie *images_serie = nullptr;
    gnomonCellImage *computed_image = nullptr;
};

gnomonSegmentationCommand::gnomonSegmentationCommand(const QString& key) : gnomonAbstractCommand<gnomonAbstractCellImageFromImage>(), d(new gnomonSegmentationCommandPrivate)
{
    QString command = "import gnomonCellImageFromImage";
    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::cellImageFromImage::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonSegmentationCommand::~gnomonSegmentationCommand(void)
{
    if (d->computed_image)
        delete d->computed_image;

    delete d;
}

void gnomonSegmentationCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->setInput(d->images_serie);
    this->action->run();
    d->computed_image = this->action->computedImage();
}

void gnomonSegmentationCommand::undo(void)
{
    Q_ASSERT(this->action);

    this->action->setInput(nullptr);
}

void gnomonSegmentationCommand::setInput(gnomonImagesSerie* images_serie)
{
    d->images_serie = images_serie;
}

gnomonImagesSerie *gnomonSegmentationCommand::input()
{
    return this->action->input();
}

gnomonImagesSerie *gnomonSegmentationCommand::output()
{
    return this->action->output();
}

QMap<QString, gnomonCoreParameter *> gnomonSegmentationCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonSegmentationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

gnomonCellImage *gnomonSegmentationCommand::computedImage(void) const
{
    return d->computed_image;
}
