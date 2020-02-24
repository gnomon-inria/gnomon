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

#include "gnomonDataFrameReaderCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonDataFrameReaderCommandPrivate
{
public:
    QString path;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonDataFrameReaderCommand::gnomonDataFrameReaderCommand(const QString& key) : d(new gnomonDataFrameReaderCommandPrivate)
{
    loadPluginGroup("dataFrameReader");

    this->action = gnomonCore::dataFrameReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonDataFrameReaderCommand::~gnomonDataFrameReaderCommand()
{
    delete d;
}

void gnomonDataFrameReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractDataFrameReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonDataFrameReaderCommand::undo(void)
{
    ((gnomonAbstractDataFrameReader *) this->action)->setPath("");
}

void gnomonDataFrameReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonDataFrameSeries *gnomonDataFrameReaderCommand::dataFrame(void)
{
    gnomonDataFrameSeries *dataFrame = ((gnomonAbstractDataFrameReader *) this->action)->dataFrame();
    if ((!dataFrame)||(dataFrame->times().size()==0)) {
        return nullptr;
    } else {
        return dataFrame;
    }
}

//
// gnomonDataFrameReaderCommand.cpp ends here
