#include "gnomonImagesFusionCommand.h"

#include <dtkScript>

class gnomonImagesFusionCommandPrivate
{
public:
    QVector<gnomonImagesSerie *> images_series;
};

gnomonImagesFusionCommand::gnomonImagesFusionCommand(const QString& key) : d(new gnomonImagesFusionCommandPrivate)
{
    QString command = "import gnomonImagesFusion";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    gnomonAbstractCommand<gnomonAbstractImagesFusion>::action = gnomonCore::imagesFusion::pluginFactory().create(key);

    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractImagesFusion>::action);
}

gnomonImagesFusionCommand::~gnomonImagesFusionCommand()
{
    delete d;
}

void gnomonImagesFusionCommand::redo(void)
{
    Q_ASSERT(gnomonAbstractCommand<gnomonAbstractImagesFusion>::action);

    for(auto& images_serie : d->images_series) {
        gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->addImagesSerie(images_serie);
    };

    gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->run();
}

void gnomonImagesFusionCommand::undo(void)
{
    gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->removeImagesSeries();
}

void gnomonImagesFusionCommand::addImagesSerie(gnomonImagesSerie *images_serie)
{
    d->images_series.push_back(images_serie);
}

QMap<QString, gnomonParameter*> gnomonImagesFusionCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonImagesFusionCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

gnomonImagesSerie *gnomonImagesFusionCommand::output(void)
{
    return gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->output();
}
