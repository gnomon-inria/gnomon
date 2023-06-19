#include "gnomonTreeWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeWriter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonTreeWriterCommandPrivate
{
public:
    std::shared_ptr<gnomonTreeSeries> tree = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeWriterCommand::gnomonTreeWriterCommand() : d(new gnomonTreeWriterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

        //see MeshWriterCommand for what to do when there is multiple writers
    QStringList keys = gnomonCore::treeWriter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeWriter::pluginFactory().create(this->algorithm_name);
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

void gnomonTreeWriterCommand::predo(void)
{
    ((gnomonAbstractTreeWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractTreeWriter *) this->action)->setTree(d->tree);
}

void gnomonTreeWriterCommand::postdo(void)
{

}

void gnomonTreeWriterCommand::undo()
{
    ((gnomonAbstractTreeWriter *) this->action)->setPath("");
}

void gnomonTreeWriterCommand::setTree(std::shared_ptr<gnomonTreeSeries> tree)
{
    d->tree = tree;
    ((gnomonAbstractTreeWriter *) this->action)->setTree(tree);
}

void gnomonTreeWriterCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    setTree(std::dynamic_pointer_cast<gnomonTreeSeries>(form));
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonTreeWriterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["tree"] = d->tree;
    return inputs;
}

bool gnomonTreeWriterCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonTreeWriterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonTreeWriterCommand::inputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("tree", "gnomonTree"));
    return types;
}

void gnomonTreeWriterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "tree") {
        this->setTree(std::dynamic_pointer_cast<gnomonTreeSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonTreeWriterCommand.cpp ends here
