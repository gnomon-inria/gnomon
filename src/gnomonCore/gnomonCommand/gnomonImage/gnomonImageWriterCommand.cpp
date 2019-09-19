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

#include "gnomonImageWriterCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageWriterCommandPrivate
{
public:
    QString path;
    gnomonImageSeries* image = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageWriterCommand::gnomonImageWriterCommand(const QString& key) : d(new gnomonImageWriterCommandPrivate)
{
    QString command = "import gnomonImageWriter";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::imageWriter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImageWriterCommand::~gnomonImageWriterCommand()
{
    delete d;
}

void gnomonImageWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractImageWriter *) this->action)->setPath(d->path);
    ((gnomonAbstractImageWriter *) this->action)->setImage(d->image);
    this->action->run();
}

void gnomonImageWriterCommand::undo(void)
{
    ((gnomonAbstractImageWriter *) this->action)->setPath("");
}

void gnomonImageWriterCommand::setPath(const QString& path)
{
    d->path = path;
}

void gnomonImageWriterCommand::setImage(gnomonImageSeries *image)
{
    d->image = image;
}

//
// gnomonImageWriterCommand.cpp ends here
