#include "gnomonDataDictWriterCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonDataDict/gnomonAbstractDataDictWriter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonDataDictWriterCommandPrivate
{
public:
    std::shared_ptr<gnomonDataDictSeries> dataDict = nullptr;
};

gnomonDataDictWriterCommand::gnomonDataDictWriterCommand() : d(new gnomonDataDictWriterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

        //see MeshWriterCommand for what to do when there is multiple writers
    QStringList keys = gnomonCore::dataDictWriter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::dataDictWriter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonDataDictWriterCommand::~gnomonDataDictWriterCommand()
{
    delete d;
}

void gnomonDataDictWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::dataDictWriter::pluginFactory().create(algo_name);
}



void gnomonDataDictWriterCommand::predo(void)
{
    ((gnomonAbstractDataDictWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractDataDictWriter *) this->action)->setDataDict(d->dataDict);
}

void gnomonDataDictWriterCommand::postdo(void)
{

}

void gnomonDataDictWriterCommand::undo()
{
    ((gnomonAbstractDataDictWriter *) this->action)->setPath("");
}

void gnomonDataDictWriterCommand::setDataDict(std::shared_ptr<gnomonDataDictSeries> dataDict)
{
    d->dataDict = dataDict;
}

void gnomonDataDictWriterCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->dataDict = std::dynamic_pointer_cast<gnomonDataDictSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonDataDictWriterCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["dataDict"] = d->dataDict;
    return inputs;
}

bool gnomonDataDictWriterCommand::isEmpty()
{
    loadPluginGroup("dataDictWriter");
    return gnomonCore::dataDictWriter::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonDataDictWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("dataDict", "gnomonDataDict"));
    return input_types;
}

void gnomonDataDictWriterCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "dataDict") {
        this->setForm(std::dynamic_pointer_cast<gnomonDataDictSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

QStringList gnomonDataDictWriterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}
