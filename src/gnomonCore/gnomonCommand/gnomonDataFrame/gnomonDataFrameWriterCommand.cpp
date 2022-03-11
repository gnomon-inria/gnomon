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
    this->factory_name = groupName;
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

void gnomonDataFrameWriterCommand::predo(void)
{
    ((gnomonAbstractDataFrameWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractDataFrameWriter *) this->action)->setDataFrame(d->dataFrame);
}

void gnomonDataFrameWriterCommand::postdo(void)
{

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
    return availablePlugins().empty();
}

QStringList gnomonDataFrameWriterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

QStringList gnomonDataFrameWriterCommand::extensions(void)
{
    return dynamic_cast<gnomonAbstractDataFrameWriter *>(this->action)->extensions();
}

gnomonAbstractCommand::orderedMap gnomonDataFrameWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("dataFrame", "gnomonDataFrame"));
    return input_types;
}

void gnomonDataFrameWriterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "dataFrame") {
        this->setDataFrame(dynamic_cast<gnomonDataFrameSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonDataFrameWriterCommand.cpp ends here
