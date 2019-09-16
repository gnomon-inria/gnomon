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

#include "gnomonCellImageReaderCommand.h"

// /////////////////////////////////////////////////////////////////////////////
// TODO: Script
// /////////////////////////////////////////////////////////////////////////////

// #include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////////////////

class gnomonCellImageReaderCommandPrivate
{
public:
    QString path;
};

// /////////////////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageReaderCommand::gnomonCellImageReaderCommand(const QString& key) : d(new gnomonCellImageReaderCommandPrivate)
{
    // QString command = "import gnomonCellImageReader";

    // int stat;

    // dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    // Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::cellImageReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellImageReaderCommand::~gnomonCellImageReaderCommand()
{
    delete d;
}

void gnomonCellImageReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonCellImageReaderCommand::undo(void)
{
    ((gnomonAbstractCellImageReader *) this->action)->setPath("");
}

void gnomonCellImageReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonCellImageSeries *gnomonCellImageReaderCommand::cellImage(void)
{
    return ((gnomonAbstractCellImageReader *) this->action)->cellImage();
}

// 
// gnomonCellImageReaderCommand.cpp ends here
