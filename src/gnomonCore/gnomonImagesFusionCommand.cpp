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

    for(auto& image : d->images) {
        gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->addImage(image);
    };

    gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->run();
}

void gnomonImagesFusionCommand::undo(void)
{
    gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->removeImages();
}

void gnomonImagesFusionCommand::addImage(dtkImage *image)
{
    d->images.push_back(image);
}

QMap<QString, gnomonParameter*> gnomonImagesFusionCommand::parameters(void) const
{
    return this->action->parameters();
}

dtkImage *gnomonImagesFusionCommand::output(void)
{
    return gnomonAbstractCommand<gnomonAbstractImagesFusion>::action->output();
}
