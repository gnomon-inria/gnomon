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

gnomonTreeWriterCommand::gnomonTreeWriterCommand(void) : d(new gnomonTreeWriterCommandPrivate)
{
    this->factory_name = "treeWriter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeFromLString::pluginFactory().keys();
    if (keys.size() > 0) {
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
    if (this->action)
        delete this->action;
    this->action = gnomonCore::treeWriter::pluginFactory().create(algo_name);
}

void gnomonTreeWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractTreeWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractTreeWriter *) this->action)->setTree(d->tree);
    this->action->run();
}

void gnomonTreeWriterCommand::undo(void)
{
    ((gnomonAbstractTreeWriter *) this->action)->setPath("");
}

void gnomonTreeWriterCommand::setPath(const QString& path)
{
    this->m_path = path;
}

void gnomonTreeWriterCommand::setTree(gnomonTreeSeries *tree)
{
    d->tree = tree;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeWriterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["tree"] = d->tree;
    return inputs;
}

bool gnomonTreeWriterCommand::isEmpty(void)
{
    loadPluginGroup("treeWriter");
    return gnomonCore::treeWriter::pluginFactory().keys().size() == 0;
}

//
// gnomonTreeWriterCommand.cpp ends here
