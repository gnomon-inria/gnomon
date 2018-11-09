#include "gnomonSegmentationCommand.h"

#include <gnomonCellImage>

#include <dtkScript>
#include <dtkImagingCore>

class gnomonSegmentationCommandPrivate
{
public:
    QMap<QString, QVariant> parameters;

public:
    dtkImage* image;
};

gnomonSegmentationCommand::gnomonSegmentationCommand(const QString& key) : d(new gnomonSegmentationCommandPrivate)
{
    QString command = "import " + key;
    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action = gnomonCore::cellimageFromImage::pluginFactory().create(key);

    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action);
}

gnomonSegmentationCommand::~gnomonSegmentationCommand()
{
    delete d;
}

void gnomonSegmentationCommand::redo(void)
{
    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action);
    gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action->setImage(d->image);

    QMap<QString, QVariant>::const_iterator i = d->parameters.constBegin();
    while (i != d->parameters.constEnd()) {
        gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action->setParameter(i.key(), i.value());
        ++i;
    }

    gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action->run();
}

void gnomonSegmentationCommand::undo(void)
{
    gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action->setImage(nullptr);
}

void gnomonSegmentationCommand::setImage(dtkImage* image)
{
    d->image = image;
}

void gnomonSegmentationCommand::setParameter(const QString& param_name, const QVariant& param_value)
{
    d->parameters[param_name] = param_value;
}

dtkImage *gnomonSegmentationCommand::computedImage(void)
{
    return gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action->computedImage()->image();
}
