#include "gnomonImageFilterCommand.h"

#include <dtkScript>

#include <dtkImagingCore>

class gnomonImageFilterCommandPrivate
{
public:
    gnomonImageSeries* input = nullptr;
};

gnomonImageFilterCommand::gnomonImageFilterCommand(const QString& key) : d(new gnomonImageFilterCommandPrivate)
{
    QString command = "import gnomonImageFilter";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

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
    return ((gnomonAbstractImageFilter *) this->action)->input();
}

gnomonImageSeries *gnomonImageFilterCommand::output(void)
{
    return ((gnomonAbstractImageFilter *) this->action)->output();
}
