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

#include "gnomonCellImageWriterCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonCellImageWriterCommandPrivate
{
public:
    QString path;

public:
    gnomonCellImageSeries* cellImage = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageWriterCommand::gnomonCellImageWriterCommand(const QString& key) : d(new gnomonCellImageWriterCommandPrivate)
{
    loadPluginGroup("cellImageWriter");

    this->action = gnomonCore::cellImageWriter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellImageWriterCommand::~gnomonCellImageWriterCommand()
{
    delete d;
}

void gnomonCellImageWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageWriter *) this->action)->setPath(d->path);
    ((gnomonAbstractCellImageWriter *) this->action)->setCellImage(d->cellImage);
    this->action->run();
}

void gnomonCellImageWriterCommand::undo(void)
{
    ((gnomonAbstractCellImageWriter *) this->action)->setPath("");
}

void gnomonCellImageWriterCommand::setPath(const QString& path)
{
    d->path = path;
}

void gnomonCellImageWriterCommand::setCellImage(gnomonCellImageSeries *cellImage)
{
    d->cellImage = cellImage;
}

//
// gnomonCellImageWriterCommand.cpp ends here
