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
    gnomonDataFrameSeries* dataFrame = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonDataFrameWriterCommand::gnomonDataFrameWriterCommand(const QString& key) : d(new gnomonDataFrameWriterCommandPrivate)
{
    loadPluginGroup("dataFrameWriter");

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

void gnomonDataFrameWriterCommand::setDataFrame(gnomonDataFrameSeries *dataFrame)
{
    d->dataFrame = dataFrame;
}

bool gnomonDataFrameWriterCommand::isEmpty(void)
{
    loadPluginGroup("dataFrameWriter");
    return gnomonCore::dataFrameWriter::pluginFactory().keys().size() == 0;
}

//
// gnomonDataFrameWriterCommand.cpp ends here
