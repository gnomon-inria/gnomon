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

gnomonDataFrameWriterCommand::gnomonDataFrameWriterCommand() : d(new gnomonDataFrameWriterCommandPrivate)
{
    this->factory_name = "dataFrameWriter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::dataFrameWriter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::dataFrameWriter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonDataFrameWriterCommand::~gnomonDataFrameWriterCommand()
{
    delete d;
}

void gnomonDataFrameWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::dataFrameWriter::pluginFactory().create(algo_name);
}

void gnomonDataFrameWriterCommand::redo()
{
    Q_ASSERT(this->action);
    ((gnomonAbstractDataFrameWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractDataFrameWriter *) this->action)->setDataFrame(d->dataFrame);
    this->action->run();
}

void gnomonDataFrameWriterCommand::undo()
{
    ((gnomonAbstractDataFrameWriter *) this->action)->setPath("");
}

void gnomonDataFrameWriterCommand::setDataFrame(gnomonDataFrameSeries *dataFrame)
{
    d->dataFrame = dataFrame;
}

void gnomonDataFrameWriterCommand::setForm(gnomonAbstractDynamicForm *form)
{
    d->dataFrame = dynamic_cast<gnomonDataFrameSeries *>(form);
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonDataFrameWriterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["dataFrame"] = d->dataFrame;
    return inputs;
}

bool gnomonDataFrameWriterCommand::isEmpty()
{
    loadPluginGroup("dataFrameWriter");
    return gnomonCore::dataFrameWriter::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonDataFrameWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("dataFrame", "gnomonDataFrame"));
    return input_types;
}

//
// gnomonDataFrameWriterCommand.cpp ends here
