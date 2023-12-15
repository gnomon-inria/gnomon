#include "gnomonCellComplexWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexWriter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonCellComplexWriterCommandPrivate
{
public:
    std::shared_ptr<gnomonCellComplexSeries> cellComplex = nullptr;
};

gnomonCellComplexWriterCommand::gnomonCellComplexWriterCommand() : d(new gnomonCellComplexWriterCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonCellComplexWriterCommand)

        //see MeshWriterCommand for what to do when there is multiple writers
    QStringList keys = gnomonCore::cellComplexWriter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellComplexWriter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellComplexWriterCommand::~gnomonCellComplexWriterCommand()
{
    delete d;
}

void gnomonCellComplexWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellComplexWriter::pluginFactory().create(algo_name);
}

void gnomonCellComplexWriterCommand::predo()
{
    Q_ASSERT(this->action);
    ((gnomonAbstractCellComplexWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractCellComplexWriter *) this->action)->setCellComplex(d->cellComplex);
}

void gnomonCellComplexWriterCommand::undo()
{
    ((gnomonAbstractCellComplexWriter *) this->action)->setPath("");
}

void gnomonCellComplexWriterCommand::setCellComplex(std::shared_ptr<gnomonCellComplexSeries> cellComplex)
{
    d->cellComplex = cellComplex;
}

void gnomonCellComplexWriterCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->cellComplex = std::dynamic_pointer_cast<gnomonCellComplexSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellComplexWriterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["cellComplex"] = d->cellComplex;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonCellComplexWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonCellComplex"));
    return input_types;
}

void gnomonCellComplexWriterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "input") {
        this->setForm(std::dynamic_pointer_cast<gnomonCellComplexSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonCellComplexWriterCommand::postdo(void) {

}

GNOMON_REGISTER_TYPE(gnomonCellComplexWriterCommand)
//
// gnomonCellComplexWriterCommand.cpp ends here
