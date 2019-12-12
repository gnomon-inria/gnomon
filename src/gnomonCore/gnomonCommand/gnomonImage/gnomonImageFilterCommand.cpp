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

#include "gnomonImageFilterCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageFilterCommandPrivate
{
public:
    gnomonImageSeries* input = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageFilterCommand::gnomonImageFilterCommand(const QString& key) : d(new gnomonImageFilterCommandPrivate)
{
    loadPluginGroup("imageFilter");

    this->action = gnomonCore::imageFilter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImageFilterCommand::~gnomonImageFilterCommand(void)
{
    delete d;
}

void gnomonImageFilterCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonImageFilterCommand::undo(void)
{
    ((gnomonAbstractImageFilter *) this->action)->setInput(nullptr);
}

void gnomonImageFilterCommand::setInput(gnomonImageSeries *input)
{
    d->input = input;

    Q_ASSERT(this->action);
    ((gnomonAbstractImageFilter *) this->action)->setInput(d->input);
}

void gnomonImageFilterCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonImageFilterCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImageSeries *gnomonImageFilterCommand::input(void)
{
    gnomonImageSeries *image = ((gnomonAbstractImageFilter *) this->action)->input();
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        return nullptr;
    } else {
        return image;
    }
}

gnomonImageSeries *gnomonImageFilterCommand::output(void)
{
    gnomonImageSeries *image = ((gnomonAbstractImageFilter *) this->action)->output();
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        return nullptr;
    } else {
        return image;
    }
}

//
// gnomonImageFilterCommand.cpp ends here
