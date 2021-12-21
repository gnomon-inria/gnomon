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
    gnomonTreeSeries* tree = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeWriterCommand::gnomonTreeWriterCommand() : d(new gnomonTreeWriterCommandPrivate)
{
    this->factory_name = "treeWriter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeFromLString::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeFromLString::pluginFactory().create(this->algorithm_name);
    }
}

gnomonTreeWriterCommand::~gnomonTreeWriterCommand()
{
    delete d;
}

void gnomonTreeWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::treeWriter::pluginFactory().create(algo_name);
}

void gnomonTreeWriterCommand::redo()
{
    Q_ASSERT(this->action);
    ((gnomonAbstractTreeWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractTreeWriter *) this->action)->setTree(d->tree);
    this->action->run();
}

void gnomonTreeWriterCommand::undo()
{
    ((gnomonAbstractTreeWriter *) this->action)->setPath("");
}

void gnomonTreeWriterCommand::setTree(gnomonTreeSeries *tree)
{
    d->tree = tree;
}

void gnomonTreeWriterCommand::setForm(gnomonAbstractDynamicForm *form)
{
    d->tree = dynamic_cast<gnomonTreeSeries *>(form);
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeWriterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["tree"] = d->tree;
    return inputs;
}

bool gnomonTreeWriterCommand::isEmpty()
{
    loadPluginGroup("treeWriter");
    return gnomonCore::treeWriter::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonTreeWriterCommand::inputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("tree", "gnomonTree"));
    return types;
}

//
// gnomonTreeWriterCommand.cpp ends here
