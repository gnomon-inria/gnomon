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

#include "gnomonTreeReaderCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonTreeReaderCommandPrivate
{
public:
    QString path;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeReaderCommand::gnomonTreeReaderCommand(const QString& key) : d(new gnomonTreeReaderCommandPrivate)
{
    QString command = "import gnomonTreeReader";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::treeReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonTreeReaderCommand::~gnomonTreeReaderCommand()
{
    delete d;
}

void gnomonTreeReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractTreeReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonTreeReaderCommand::undo(void)
{
    ((gnomonAbstractTreeReader *) this->action)->setPath("");
}

void gnomonTreeReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonTreeSeries *gnomonTreeReaderCommand::tree(void)
{
    return ((gnomonAbstractTreeReader *) this->action)->tree();
}

//
// gnomonTreeReaderCommand.cpp ends here
