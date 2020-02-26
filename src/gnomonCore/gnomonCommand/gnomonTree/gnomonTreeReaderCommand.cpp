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

#include "gnomonTreeReaderCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonTreeReaderCommandPrivate
{
public:
    QString path;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeReaderCommand::gnomonTreeReaderCommand(const QString& key) : d(new gnomonTreeReaderCommandPrivate)
{
    loadPluginGroup("treeReader");

    this->action = gnomonCore::treeReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonTreeReaderCommand::~gnomonTreeReaderCommand()
{
    delete d;
}

void gnomonTreeReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractTreeReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonTreeReaderCommand::undo(void)
{
    ((gnomonAbstractTreeReader *) this->action)->setPath("");
}

void gnomonTreeReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonTreeSeries *gnomonTreeReaderCommand::tree(void)
{
    gnomonTreeSeries *tree = ((gnomonAbstractTreeReader *) this->action)->tree();
    if ((!tree)||(tree->times().size()==0)) {
        return nullptr;
    } else {
        return tree;
    }
}

bool gnomonTreeReaderCommand::isEmpty(void)
{
    loadPluginGroup("treeReader");
    return gnomonCore::treeReader::pluginFactory().keys().size() == 0;
}

//
// gnomonTreeReaderCommand.cpp ends here
