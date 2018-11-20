#include "gnomonImagesFusionCommand.h"

#include <dtkScript>

class gnomonImagesFusionCommandPrivate
{
public:
    QVector<dtkImage *> images;
    QMap<QString, QVariant> parameters;
};

gnomonImagesFusionCommand::gnomonImagesFusionCommand(const QString& key) : d(new gnomonImagesFusionCommandPrivate)
{
    QString command = "import " + key;

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

    gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->setParameter("nb_iterations", d->parameters["nb_iterations"]);

    for(auto& image : d->images) {
        gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->addImage(image);
    };

    gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->run();
}

void gnomonImagesFusionCommand::undo(void)
{
    gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->removeImages();
}

void gnomonImagesFusionCommand::setParameters(const QMap<QString, QVariant>& parameters)
{
    d->parameters = parameters;
}

void gnomonImagesFusionCommand::setParameter(const QString& parameterName, const QVariant& parameterValue)
{
    d->parameters[parameterName] = parameterValue;
}

void gnomonImagesFusionCommand::addImage(dtkImage *image)
{
    d->images.push_back(image);
}

dtkImage *gnomonImagesFusionCommand::output(void)
{
    return gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->output();
}
