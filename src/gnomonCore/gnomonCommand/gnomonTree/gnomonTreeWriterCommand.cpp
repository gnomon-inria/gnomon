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

#include "gnomonTreeWriterCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonTreeWriterCommandPrivate
{
public:
    QString path;

public:
    gnomonTreeSeries* tree = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeWriterCommand::gnomonTreeWriterCommand(const QString& key) : d(new gnomonTreeWriterCommandPrivate)
{
    loadPluginGroup("treeWriter");

    this->action = gnomonCore::treeWriter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonTreeWriterCommand::~gnomonTreeWriterCommand()
{
    delete d;
}

void gnomonTreeWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractTreeWriter *) this->action)->setPath(d->path);
    ((gnomonAbstractTreeWriter *) this->action)->setInput(d->tree);
    this->action->run();
}

void gnomonTreeWriterCommand::undo(void)
{
    ((gnomonAbstractTreeWriter *) this->action)->setPath("");
}

void gnomonTreeWriterCommand::setPath(const QString& path)
{
    qDebug() << Q_FUNC_INFO << "LA COMMAND PATH";
    d->path = path;
}

void gnomonTreeWriterCommand::setInput(gnomonTreeSeries *tree)
{
    qDebug() << Q_FUNC_INFO << "LA COMMAND TREE";
    d->tree = tree;
    qDebug() << Q_FUNC_INFO << "LA COMMAND TREE APRES";
}

bool gnomonTreeWriterCommand::isEmpty(void)
{
    loadPluginGroup("treeWriter");
    return gnomonCore::treeWriter::pluginFactory().keys().size() == 0;
}

//
// gnomonTreeWriterCommand.cpp ends here
