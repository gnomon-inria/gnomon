#include "gnomonImagesRegistrationCommand.h"

#include <dtkScript>

class gnomonImagesRegistrationCommandPrivate
{
public:
    QVector<gnomonImagesSerie *> images_series;
};

gnomonImagesRegistrationCommand::gnomonImagesRegistrationCommand(const QString& key) : d(new gnomonImagesRegistrationCommandPrivate)
{
    QString command = "import gnomonImagesRegistration";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::imagesRegistration::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImagesRegistrationCommand::~gnomonImagesRegistrationCommand()
{
    delete d;
}

void gnomonImagesRegistrationCommand::redo(void)
{
    Q_ASSERT(this->action);

    for(auto& images_serie : d->images_series) {
        ((gnomonAbstractImagesRegistration *) this->action)->addImagesSerie(images_serie);
    };

    this->action->run();
}

void gnomonImagesRegistrationCommand::undo(void)
{
    ((gnomonAbstractImagesRegistration *) this->action)->removeImagesSeries();
    d->images_series.clear();
}

void gnomonImagesRegistrationCommand::addImagesSerie(gnomonImagesSerie *images_serie)
{
    d->images_series.push_back(images_serie);
}

gnomonImagesSerie* gnomonImagesRegistrationCommand::output()
{ return ((gnomonAbstractImagesRegistration *) this->action)->output(); }

QMap<QString, gnomonCoreParameter*> gnomonImagesRegistrationCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonImagesRegistrationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}