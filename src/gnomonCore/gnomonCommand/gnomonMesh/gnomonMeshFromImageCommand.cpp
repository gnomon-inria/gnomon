// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonMeshFromImageCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonMeshFromImageCommandPrivate
{
public:
    gnomonImageSeries* input = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshFromImageCommand::gnomonMeshFromImageCommand(const QString& key) : d(new gnomonMeshFromImageCommandPrivate)
{
    QString command = "import gnomonMeshFromImage";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::meshFromImage::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonMeshFromImageCommand::~gnomonMeshFromImageCommand(void)
{
    delete d;
}

void gnomonMeshFromImageCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonMeshFromImageCommand::undo(void)
{
    ((gnomonAbstractMeshFromImage *) this->action)->setInput(nullptr);
}

void gnomonMeshFromImageCommand::setInput(gnomonImageSeries *input)
{
    d->input = input;

    Q_ASSERT(this->action);
    ((gnomonAbstractMeshFromImage *) this->action)->setInput(d->input);
}

void gnomonMeshFromImageCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

QMap<QString, gnomonCoreParameter *> gnomonMeshFromImageCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonImageSeries *gnomonMeshFromImageCommand::input(void)
{
    return ((gnomonAbstractMeshFromImage *) this->action)->input();
}

gnomonMeshSeries *gnomonMeshFromImageCommand::output(void)
{
    return ((gnomonAbstractMeshFromImage *) this->action)->output();
}

//
// gnomonMeshFromImageCommand.cpp ends here
