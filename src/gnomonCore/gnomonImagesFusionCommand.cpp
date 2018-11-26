#include "gnomonImagesFusionCommand.h"

#include <dtkScript>

class gnomonImagesFusionCommandPrivate
{
public:
    QVector<dtkImage *> images;
};

gnomonImagesFusionCommand::gnomonImagesFusionCommand(const QString& key) : d(new gnomonImagesFusionCommandPrivate)
{
    QString command = "import gnomonImagesFusion";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::imagesFusion::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImagesFusionCommand::~gnomonImagesFusionCommand(void)
{
    delete d;
}

void gnomonImagesFusionCommand::redo(void)
{
    Q_ASSERT(this->action);

    for(auto& image : d->images) {
        this->action->addImage(image);
    };

    this->action->run();
}

void gnomonImagesFusionCommand::undo(void)
{
    this->action->removeImages();
}

void gnomonImagesFusionCommand::addImage(dtkImage *image)
{
    d->images.push_back(image);
}

void gnomonImagesFusionCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonImagesFusionCommand::parameters(void) const
{
    return this->action->parameters();
}

dtkImage *gnomonImagesFusionCommand::output(void)
{
    return this->action->output();
}
