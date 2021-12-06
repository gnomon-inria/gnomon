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

gnomonTreeReaderCommand::gnomonTreeReaderCommand(void) : d(new gnomonTreeReaderCommandPrivate)
{
    this->factory_name = "treeReader";
    loadPluginGroup(this->factoryName());

    for (auto key: gnomonCore::treeReader::pluginFactory().keys()) {
        auto algo = gnomonCore::treeReader::pluginFactory().create(key);
        if (!this->action) {
            this->action = algo;
            this->algorithm_name = key;
        }
        m_descriptions.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
    }
}

gnomonTreeReaderCommand::~gnomonTreeReaderCommand()
{
    this->action = nullptr;
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
