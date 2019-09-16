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

#include "gnomonImageReaderCommand.h"

// /////////////////////////////////////////////////////////////////////////////
// TODO: Script
// /////////////////////////////////////////////////////////////////////////////

// #include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageReaderCommandPrivate
{
public:
    QString path;
};

// /////////////////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////////////////

gnomonImageReaderCommand::gnomonImageReaderCommand(const QString& key) : d(new gnomonImageReaderCommandPrivate)
{
    // QString command = "import gnomonImageReader";

    // int stat;

    // dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    // Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::imageReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImageReaderCommand::~gnomonImageReaderCommand()
{
    delete d;
}

void gnomonImageReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractImageReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonImageReaderCommand::undo(void)
{
    ((gnomonAbstractImageReader *) this->action)->setPath("");
}

void gnomonImageReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonImageSeries *gnomonImageReaderCommand::image(void)
{
    return ((gnomonAbstractImageReader *) this->action)->image();
}

// 
// gnomonImageReaderCommand.cpp ends here
