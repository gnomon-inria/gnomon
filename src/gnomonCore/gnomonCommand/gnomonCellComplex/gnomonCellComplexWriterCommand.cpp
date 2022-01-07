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

#include "gnomonCellComplexWriterCommand.h"

#include <dtkScript>

class gnomonCellComplexWriterCommandPrivate
{
public:
    gnomonCellComplexSeries *cellComplex = nullptr;
};

gnomonCellComplexWriterCommand::gnomonCellComplexWriterCommand() : d(new gnomonCellComplexWriterCommandPrivate)
{
    this->factory_name = "cellComplexWriter";
    loadPluginGroup(this->factoryName());

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

void gnomonCellComplexWriterCommand::redo()
{
    Q_ASSERT(this->action);
    ((gnomonAbstractCellComplexWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractCellComplexWriter *) this->action)->setCellComplex(d->cellComplex);
    this->action->run();
}

void gnomonCellComplexWriterCommand::undo()
{
    ((gnomonAbstractCellComplexWriter *) this->action)->setPath("");
}

void gnomonCellComplexWriterCommand::setCellComplex(gnomonCellComplexSeries *cellComplex)
{
    d->cellComplex = cellComplex;
}

void gnomonCellComplexWriterCommand::setForm(gnomonAbstractDynamicForm *form)
{
    d->cellComplex = dynamic_cast<gnomonCellComplexSeries *>(form);
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexWriterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["cellComplex"] = d->cellComplex;
    return inputs;
}

bool gnomonCellComplexWriterCommand::isEmpty()
{
    loadPluginGroup("cellComplexWriter");
    return gnomonCore::cellComplexWriter::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonCellComplexWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonCellComplex"));
    return input_types;
}

void gnomonCellComplexWriterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "input") {
        this->setForm(dynamic_cast<gnomonCellComplexSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonCellComplexWriterCommand.cpp ends here
