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
    gnomonCellImageSeries* cellImage = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageWriterCommand::gnomonCellImageWriterCommand(const QString& key) : d(new gnomonCellImageWriterCommandPrivate)
{
    this->factory_name = "cellImageWriter";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
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
    ((gnomonAbstractCellImageWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractCellImageWriter *) this->action)->setCellImage(d->cellImage);
    this->action->run();
}

void gnomonCellImageWriterCommand::undo(void)
{
    ((gnomonAbstractCellImageWriter *) this->action)->setPath("");
}

void gnomonCellImageWriterCommand::setPath(const QString& path)
{
    this->m_path = path;
}

void gnomonCellImageWriterCommand::setCellImage(gnomonCellImageSeries *cellImage)
{
    d->cellImage = cellImage;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageWriterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["cellImage"] = d->cellImage;
    return inputs;
}

bool gnomonCellImageWriterCommand::isEmpty(void)
{
    loadPluginGroup("cellImageWriter");
    return gnomonCore::cellImageWriter::pluginFactory().keys().size() == 0;
}

//
// gnomonCellImageWriterCommand.cpp ends here
