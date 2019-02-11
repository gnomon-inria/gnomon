#include "gnomonCellImageQuantificationCommand.h"

#include <dtkScript>

#include <dtkImagingCore>

class gnomonCellImageQuantificationCommandPrivate
{
public:
    gnomonImagesSerie* image = nullptr;
    gnomonCellImage* cellImage = nullptr;
};

gnomonCellImageQuantificationCommand::gnomonCellImageQuantificationCommand(const QString& key) : d(new gnomonCellImageQuantificationCommandPrivate)
{
    QString command = "import gnomonCellImageQuantification";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::cellImageQuantification::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellImageQuantificationCommand::~gnomonCellImageQuantificationCommand(void)
{
    delete d;
}

void gnomonCellImageQuantificationCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonCellImageQuantificationCommand::undo(void)
{
    ((gnomonAbstractCellImageQuantification *) this->action)->setImage(nullptr);
    ((gnomonAbstractCellImageQuantification *) this->action)->setCellImage(nullptr);
}

void gnomonCellImageQuantificationCommand::setImage(gnomonImagesSerie *image)
{
    d->image = image;

    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageQuantification *) this->action)->setImage(d->image);
}


void gnomonCellImageQuantificationCommand::setCellImage(gnomonCellImage *cellImage)
{
    d->cellImage = cellImage;

    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageQuantification *) this->action)->setCellImage(d->cellImage);
}

void gnomonCellImageQuantificationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonCellImageQuantificationCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImage *gnomonCellImageQuantificationCommand::cellImage(void)
{
    return ((gnomonAbstractCellImageQuantification *) this->action)->cellImage();
}
