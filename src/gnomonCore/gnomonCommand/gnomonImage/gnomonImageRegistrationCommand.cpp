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

#include "gnomonImageRegistrationCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageRegistrationCommandPrivate
{
public:
    QVector<gnomonImageSeries *> images_series;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageRegistrationCommand::gnomonImageRegistrationCommand(const QString& key) : d(new gnomonImageRegistrationCommandPrivate)
{
    loadPluginGroup("imageRegistration");

    this->action = gnomonCore::imageRegistration::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImageRegistrationCommand::~gnomonImageRegistrationCommand()
{
    delete d;
}

void gnomonImageRegistrationCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonImageRegistrationCommand::undo(void)
{
    d->images_series.clear();
}

void gnomonImageRegistrationCommand::addImage(gnomonImageSeries *image_series)
{
    d->images_series.push_back(image_series);

    ((gnomonAbstractImageRegistration *) this->action)->removeImages();
    for(auto& images_serie : d->images_series) {
        ((gnomonAbstractImageRegistration *) this->action)->addImage(images_serie);
    };
}

gnomonImageSeries* gnomonImageRegistrationCommand::output()
{
    gnomonImageSeries *image = ((gnomonAbstractImageRegistration *) this->action)->output();
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        return nullptr;
    } else {
        return image;
    }
}

QMap<QString, gnomonCoreParameter*> gnomonImageRegistrationCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonImageRegistrationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

//
// gnomonImageRegistrationCommand.cpp ends here
