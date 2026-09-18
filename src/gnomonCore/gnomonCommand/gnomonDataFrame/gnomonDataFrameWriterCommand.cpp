#include "gnomonDataFrameWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonDataFrame/gnomonAbstractDataFrameWriter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonDataFrameWriterCommandPrivate
{
public:
    std::shared_ptr<gnomonDataFrameSeries> dataFrame = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonDataFrameWriterCommand::gnomonDataFrameWriterCommand() : d(new gnomonDataFrameWriterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonDataFrameWriterCommand)

        //see MeshWriterCommand for what to do when there is multiple writers
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
    if (d->dataFrame) {
        d->dataFrame->load();
    }
    ((gnomonAbstractDataFrameWriter *) this->action)->setDataFrame(d->dataFrame);
}

void gnomonDataFrameWriterCommand::postdo(void)
{

}

void gnomonDataFrameWriterCommand::undo()
{
    ((gnomonAbstractDataFrameWriter *) this->action)->setPath("");
}

void gnomonDataFrameWriterCommand::setDataFrame(std::shared_ptr<gnomonDataFrameSeries> dataFrame)
{
    d->dataFrame = dataFrame;
}

void gnomonDataFrameWriterCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->dataFrame = std::dynamic_pointer_cast<gnomonDataFrameSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonDataFrameWriterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["dataFrame"] = d->dataFrame;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonDataFrameWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("dataFrame", "gnomonDataFrame"));
    return input_types;
}

void gnomonDataFrameWriterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "dataFrame") {
        this->setDataFrame(std::dynamic_pointer_cast<gnomonDataFrameSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonDataFrameWriterCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->dataFrame = nullptr;
}

GNOMON_REGISTER_TYPE(gnomonDataFrameWriterCommand)

//
// gnomonDataFrameWriterCommand.cpp ends here
