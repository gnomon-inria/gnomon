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

gnomonCellComplexWriterCommand::gnomonCellComplexWriterCommand(void) : d(new gnomonCellComplexWriterCommandPrivate)
{
    this->factory_name = "cellComplexWriter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellComplexWriter::pluginFactory().keys();
    if (keys.size() > 0) {
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
    if (this->action)
        delete this->action;
    this->action = gnomonCore::cellComplexWriter::pluginFactory().create(algo_name);
}

void gnomonCellComplexWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractCellComplexWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractCellComplexWriter *) this->action)->setCellComplex(d->cellComplex);
    this->action->run();
}

void gnomonCellComplexWriterCommand::undo(void)
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

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellComplexWriterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["cellComplex"] = d->cellComplex;
    return inputs;
}

bool gnomonCellComplexWriterCommand::isEmpty(void)
{
    loadPluginGroup("cellComplexWriter");
    return gnomonCore::cellComplexWriter::pluginFactory().keys().size() == 0;
}

//
// gnomonCellComplexWriterCommand.cpp ends here
