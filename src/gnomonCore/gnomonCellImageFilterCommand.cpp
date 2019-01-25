#include "gnomonCellImageFilterCommand.h"

#include <dtkScript>

#include <dtkImagingCore>

class gnomonCellImageFilterCommandPrivate
{
public:
    gnomonCellImage* input = nullptr;
};

gnomonCellImageFilterCommand::gnomonCellImageFilterCommand(const QString& key) : d(new gnomonCellImageFilterCommandPrivate)
{
    QString command = "import gnomonCellImageFilter";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::cellImageFilter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellImageFilterCommand::~gnomonCellImageFilterCommand(void)
{
    delete d;
}

void gnomonCellImageFilterCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonCellImageFilterCommand::undo(void)
{
    ((gnomonAbstractCellImageFilter *) this->action)->setInput(nullptr);
}

void gnomonCellImageFilterCommand::setInput(gnomonCellImage *input)
{
    d->input = input;

    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageFilter *) this->action)->setInput(d->input);
}

void gnomonCellImageFilterCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonCellImageFilterCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonCellImage *gnomonCellImageFilterCommand::input(void)
{
    return ((gnomonAbstractCellImageFilter *) this->action)->input();
}

gnomonCellImage *gnomonCellImageFilterCommand::output(void)
{
    return ((gnomonAbstractCellImageFilter *) this->action)->output();
}
