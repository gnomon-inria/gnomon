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

gnomonTreeReaderCommand::gnomonTreeReaderCommand() : d(new gnomonTreeReaderCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    for (const auto& key: gnomonCore::treeReader::pluginFactory().keys()) {
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

void gnomonTreeReaderCommand::redo()
{
    Q_ASSERT(this->action);
    ((gnomonAbstractTreeReader *) this->action)->setPath(this->m_path);
    this->action->run();
    gnomonTreeSeries *tree = ((gnomonAbstractTreeReader *) this->action)->tree();
    if ((!tree)||(tree->times().empty())) {
        d->tree = nullptr;
    } else {
        d->tree = tree;
    }
}

void gnomonTreeReaderCommand::undo()
{
    ((gnomonAbstractTreeReader *) this->action)->setPath("");
}

gnomonTreeSeries *gnomonTreeReaderCommand::tree()
{
    return d->tree;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeReaderCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["tree"] = this->tree();
    return outputs;
}

bool gnomonTreeReaderCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonTreeReaderCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonTreeReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("tree", "gnomonTree"));
    return types;
}

//
// gnomonTreeReaderCommand.cpp ends here
