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
    QString path;

public:
    gnomonCellComplexSeries* cellComplex = nullptr;
};

gnomonCellComplexWriterCommand::gnomonCellComplexWriterCommand(const QString& key) : d(new gnomonCellComplexWriterCommandPrivate)
{
    loadPluginGroup("cellComplexWriter");

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
    ((gnomonAbstractCellComplexWriter *) this->action)->setPath(d->path);
    ((gnomonAbstractCellComplexWriter *) this->action)->setCellComplex(d->cellComplex);
    this->action->run();
}

void gnomonCellComplexWriterCommand::undo(void)
{
    ((gnomonAbstractCellComplexWriter *) this->action)->setPath("");
}

void gnomonCellComplexWriterCommand::setPath(const QString& path)
{
    d->path = path;
}

void gnomonCellComplexWriterCommand::setCellComplex(gnomonCellComplexSeries *cellComplex)
{
    d->cellComplex = cellComplex;
}

//
// gnomonCellComplexWriterCommand.cpp ends here
