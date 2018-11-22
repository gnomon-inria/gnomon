#include "gnomonImagesRegistrationCommand.h"

#include <dtkScript>

class gnomonImagesRegistrationCommandPrivate
{
public:
    QVector<dtkImage *> images;
    QMap<QString, QVariant> parameters;
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

    for(auto& image : d->images) {
        gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action->addImage(image);
    };

    gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action->run();
}

void gnomonImagesRegistrationCommand::undo(void)
{
    gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action->removeImages();
}

void gnomonImagesRegistrationCommand::addImage(dtkImage *image)
{
    d->images.push_back(image);
}

QMap<QString, gnomonParameter*> gnomonImagesRegistrationCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonImagesRegistrationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

double gnomonImagesRegistrationCommand::time(void)
{
    return gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action->time();
}

dtkImage *gnomonImagesRegistrationCommand::at(double t)
{
    return gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action->at(t);
}

dtkImage *gnomonImagesRegistrationCommand::next(void)
{
    return gnomonAbstractCommand<gnomonAbstractImagesRegistration>::action->next();
}
