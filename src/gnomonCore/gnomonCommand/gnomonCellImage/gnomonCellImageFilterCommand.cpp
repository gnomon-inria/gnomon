// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonCellImageFilterCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonCellImageFilterCommandPrivate
{
public:
    gnomonCellImageSeries* input = nullptr;
};

gnomonCellImageFilterCommand::gnomonCellImageFilterCommand(const QString& key) : d(new gnomonCellImageFilterCommandPrivate)
{
    loadPluginGroup("cellImageFilter");

    this->action = gnomonCore::cellImageFilter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellImageFilterCommand::~gnomonCellImageFilterCommand(void)
{
    delete d;
}

void gnomonCellImageFilterCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonCellImageFilterCommand::undo(void)
{
    ((gnomonAbstractCellImageFilter *) this->action)->setInput(nullptr);
}

void gnomonCellImageFilterCommand::setInput(gnomonCellImageSeries *input)
{
    d->input = input;

    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageFilter *) this->action)->setInput(d->input);
}

void gnomonCellImageFilterCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonCellImageFilterCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImageSeries *gnomonCellImageFilterCommand::input(void)
{
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageFilter *) this->action)->input();
    if ((!cellImage)||(cellImage->times().size()==0)) {
        return nullptr;
    } else {
        return cellImage;
    }
}

gnomonCellImageSeries *gnomonCellImageFilterCommand::output(void)
{
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageFilter *) this->action)->output();
    if ((!cellImage)||(cellImage->times().size()==0)) {
        return nullptr;
    } else {
        return cellImage;
    }
}

//
// gnomonCellImageFilterCommand.cpp ends here
