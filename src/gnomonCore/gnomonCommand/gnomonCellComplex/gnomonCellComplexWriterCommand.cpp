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

gnomonCellComplexWriterCommand::gnomonCellComplexWriterCommand(const QString& key) : d(new gnomonCellComplexWriterCommandPrivate)
{
    this->factory_name = "cellComplexWriter";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::cellComplexWriter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellComplexWriterCommand::~gnomonCellComplexWriterCommand()
{
    delete d;
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

void gnomonCellComplexWriterCommand::setPath(const QString& path)
{
    this->m_path = path;
}

void gnomonCellComplexWriterCommand::setCellComplex(gnomonCellComplexSeries *cellComplex)
{
    d->cellComplex = cellComplex;
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
