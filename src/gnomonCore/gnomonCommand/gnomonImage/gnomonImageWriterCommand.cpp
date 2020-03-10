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

#include "gnomonImageWriterCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageWriterCommandPrivate
{
public:
    QString path;
    gnomonImageSeries* image = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageWriterCommand::gnomonImageWriterCommand(const QString& key) : d(new gnomonImageWriterCommandPrivate)
{
    loadPluginGroup("imageWriter");

    this->action = gnomonCore::imageWriter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImageWriterCommand::~gnomonImageWriterCommand()
{
    delete d;
}

void gnomonImageWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractImageWriter *) this->action)->setPath(d->path);
    ((gnomonAbstractImageWriter *) this->action)->setImage(d->image);
    this->action->run();
}

void gnomonImageWriterCommand::undo(void)
{
    ((gnomonAbstractImageWriter *) this->action)->setPath("");
}

void gnomonImageWriterCommand::setPath(const QString& path)
{
    d->path = path;
}

void gnomonImageWriterCommand::setImage(gnomonImageSeries *image)
{
    d->image = image;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageWriterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["image"] = d->image;
    return inputs;
}

bool gnomonImageWriterCommand::isEmpty(void)
{
    loadPluginGroup("imageWriter");
    return gnomonCore::imageWriter::pluginFactory().keys().size() == 0;
}

//
// gnomonImageWriterCommand.cpp ends here
