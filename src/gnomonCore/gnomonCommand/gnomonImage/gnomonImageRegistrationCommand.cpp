#include "gnomonImageRegistrationCommand.h"

#include <dtkScript>

class gnomonImageRegistrationCommandPrivate
{
public:
    QVector<gnomonImageSeries *> images_series;
};

gnomonImageRegistrationCommand::gnomonImageRegistrationCommand(const QString& key) : d(new gnomonImageRegistrationCommandPrivate)
{
    QString command = "import gnomonImageRegistration";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

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
    return ((gnomonAbstractImageRegistration *) this->action)->output();
}

QMap<QString, gnomonCoreParameter*> gnomonImageRegistrationCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonImageRegistrationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}