#include "gnomonLStringWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringWriter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonLStringWriterCommandPrivate
{
public:
    std::shared_ptr<gnomonLStringSeries> lString = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonLStringWriterCommand::gnomonLStringWriterCommand() : d(new gnomonLStringWriterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::lStringWriter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::lStringWriter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonLStringWriterCommand::~gnomonLStringWriterCommand()
{
    delete d;
}

void gnomonLStringWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::lStringWriter::pluginFactory().create(algo_name);
}

void gnomonLStringWriterCommand::predo(void)
{
    ((gnomonAbstractLStringWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractLStringWriter *) this->action)->setLString(d->lString);
}

void gnomonLStringWriterCommand::postdo(void)
{

}

void gnomonLStringWriterCommand::undo()
{
    ((gnomonAbstractLStringWriter *) this->action)->setPath("");
}

void gnomonLStringWriterCommand::setLString(std::shared_ptr<gnomonLStringSeries> lString)
{
    d->lString = lString;
    ((gnomonAbstractLStringWriter *) this->action)->setLString(lString);
}

void gnomonLStringWriterCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    setLString(std::dynamic_pointer_cast<gnomonLStringSeries>(form));
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringWriterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["lString"] = d->lString;
    return inputs;
}

bool gnomonLStringWriterCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonLStringWriterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonLStringWriterCommand::inputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("lString", "gnomonLString"));
    return types;
}

void gnomonLStringWriterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "lString") {
        this->setLString(std::dynamic_pointer_cast<gnomonLStringSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonLStringWriterCommand.cpp ends here
