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

#include "gnomonDataFrameWriterCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonDataFrameWriterCommandPrivate
{
public:
    QString path;

public:
    gnomonDataFrame* dataFrame = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonDataFrameWriterCommand::gnomonDataFrameWriterCommand(const QString& key) : d(new gnomonDataFrameWriterCommandPrivate)
{
    QString command = "import gnomonDataFrameWriter";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::dataFrameWriter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonDataFrameWriterCommand::~gnomonDataFrameWriterCommand()
{
    delete d;
}

void gnomonDataFrameWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractDataFrameWriter *) this->action)->setPath(d->path);
    ((gnomonAbstractDataFrameWriter *) this->action)->setDataFrame(d->dataFrame);
    this->action->run();
}

void gnomonDataFrameWriterCommand::undo(void)
{
    ((gnomonAbstractDataFrameWriter *) this->action)->setPath("");
}

void gnomonDataFrameWriterCommand::setPath(const QString& path)
{
    d->path = path;
}

void gnomonDataFrameWriterCommand::setDataFrame(gnomonDataFrame *dataFrame)
{
    d->dataFrame = dataFrame;
}

//
// gnomonDataFrameWriterCommand.cpp ends here
