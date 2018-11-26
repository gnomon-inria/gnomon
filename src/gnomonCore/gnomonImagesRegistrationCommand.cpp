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

    gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action = gnomonCore::imagesRegistration::pluginFactory().create(key);

    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action);
}

gnomonImagesRegistrationCommand::~gnomonImagesRegistrationCommand()
{
    delete d;
}

void gnomonImagesRegistrationCommand::redo(void)
{
    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action);

    for(auto& images_serie : d->images_series) {
        gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action->addImagesSerie(images_serie);
    };

    gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action->run();
}

void gnomonImagesRegistrationCommand::undo(void)
{
    d->images_series.clear();
    gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action->removeImagesSeries();
}

void gnomonImagesRegistrationCommand::addImagesSerie(gnomonImagesSerie *images_serie)
{
    d->images_series.push_back(images_serie);
}

gnomonImagesSerie* gnomonImagesRegistrationCommand::output()
{ return this->action->output(); }

QMap<QString, gnomonCoreParameter*> gnomonImagesRegistrationCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonImagesRegistrationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}