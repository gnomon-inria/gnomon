#include "gnomonSegmentationCommand.h"

#include <gnomonCellImage>

#include <dtkScript>
#include <dtkImagingCore>

class gnomonSegmentationCommandPrivate
{
public:
    QMap<QString, QVariant> parameters;

public:
    dtkImage *image;
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

    this->action->setImage(d->image);
    for (auto it = d->parameters.cbegin(); it != d->parameters.cend(); ++it) {
        this->action->setParameter(it.key(), *it);
    }
    this->action->run();
}

void gnomonSegmentationCommand::undo(void)
{
    Q_ASSERT(this->action);

    this->action->setImage(nullptr);
}

void gnomonSegmentationCommand::setImage(dtkImage* image)
{
    d->image = image;
}

void gnomonSegmentationCommand::setParameter(const QString& param_name, const QVariant& param_value)
{
    d->parameters[param_name] = param_value;
}

QMap<QString, QVariant> gnomonSegmentationCommand::parameters(void) const
{
    return gnomonAbstractCommand<gnomonAbstractCellImageFromImage>::action->parameters();
}

gnomonCellImage *gnomonSegmentationCommand::computedImage(void) const
{
    return this->action->computedImage();
}
