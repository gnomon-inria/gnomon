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
    gnomonDataFrameSeries* dataFrame = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonDataFrameWriterCommand::gnomonDataFrameWriterCommand(const QString& key) : d(new gnomonDataFrameWriterCommandPrivate)
{
    this->factory_name = "dataFrameWriter";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
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
    ((gnomonAbstractDataFrameWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractDataFrameWriter *) this->action)->setDataFrame(d->dataFrame);
    this->action->run();
}

void gnomonDataFrameWriterCommand::undo(void)
{
    ((gnomonAbstractDataFrameWriter *) this->action)->setPath("");
}

void gnomonDataFrameWriterCommand::setPath(const QString& path)
{
    this->m_path = path;
}

void gnomonDataFrameWriterCommand::setDataFrame(gnomonDataFrameSeries *dataFrame)
{
    d->dataFrame = dataFrame;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonDataFrameWriterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["dataFrame"] = d->dataFrame;
    return inputs;
}

bool gnomonDataFrameWriterCommand::isEmpty(void)
{
    loadPluginGroup("dataFrameWriter");
    return gnomonCore::dataFrameWriter::pluginFactory().keys().size() == 0;
}

//
// gnomonDataFrameWriterCommand.cpp ends here
