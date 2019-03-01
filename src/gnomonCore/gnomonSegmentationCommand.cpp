#include "gnomonSegmentationCommand.h"

#include "gnomonForm/gnomonCellImage.h"

#include "gnomonForm/gnomonImagesSerie.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonSegmentationCommandPrivate
{
public:
    gnomonImagesSerie *images_serie = nullptr;
};

gnomonSegmentationCommand::gnomonSegmentationCommand(const QString& key) : d(new gnomonSegmentationCommandPrivate)
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
    delete d;
}

void gnomonSegmentationCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonSegmentationCommand::undo(void)
{
    Q_ASSERT(this->action);

    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(nullptr);
}

void gnomonSegmentationCommand::setInput(gnomonImagesSerie* images_serie)
{
    d->images_serie = images_serie;
    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(d->images_serie);
}

gnomonImagesSerie *gnomonSegmentationCommand::input()
{
    return ((gnomonAbstractCellImageFromImage *) this->action)->input();
}

gnomonCellImage *gnomonSegmentationCommand::output()
{
    return ((gnomonAbstractCellImageFromImage *) this->action)->computedImage();
}

QMap<QString, gnomonCoreParameter *> gnomonSegmentationCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonSegmentationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}
