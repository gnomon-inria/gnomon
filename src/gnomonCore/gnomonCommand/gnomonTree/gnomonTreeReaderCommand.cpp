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
    gnomonTreeSeries *tree = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeReaderCommand::gnomonTreeReaderCommand(const QString& key) : d(new gnomonTreeReaderCommandPrivate)
{
    this->factory_name = "treeReader";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
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
    ((gnomonAbstractTreeReader *) this->action)->setPath(this->m_path);
    this->action->run();
    gnomonTreeSeries *tree = ((gnomonAbstractTreeReader *) this->action)->tree();
    if ((!tree)||(tree->times().size()==0)) {
        d->tree = nullptr;
    } else {
        d->tree = tree;
    }
}

void gnomonTreeReaderCommand::undo(void)
{
    ((gnomonAbstractTreeReader *) this->action)->setPath("");
}

void gnomonTreeReaderCommand::setPath(const QString& path)
{
    this->m_path = path;
}

gnomonTreeSeries *gnomonTreeReaderCommand::tree(void)
{
    return d->tree;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeReaderCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["tree"] = this->tree();
    return outputs;
}

bool gnomonTreeReaderCommand::isEmpty(void)
{
    loadPluginGroup("treeReader");
    return gnomonCore::treeReader::pluginFactory().keys().size() == 0;
}

//
// gnomonTreeReaderCommand.cpp ends here
