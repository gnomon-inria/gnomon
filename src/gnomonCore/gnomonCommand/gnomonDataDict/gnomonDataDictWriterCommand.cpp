#include "gnomonDataDictWriterCommand.h"

#include <dtkScript>

class gnomonDataDictWriterCommandPrivate
{
public:
    gnomonDataDictSeries *dataDict = nullptr;
};

gnomonDataDictWriterCommand::gnomonDataDictWriterCommand() : d(new gnomonDataDictWriterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

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

void gnomonDataDictWriterCommand::redo()
{
    Q_ASSERT(this->action);
    ((gnomonAbstractDataDictWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractDataDictWriter *) this->action)->setDataDict(d->dataDict);
    this->action->run();
}

void gnomonDataDictWriterCommand::undo()
{
    ((gnomonAbstractDataDictWriter *) this->action)->setPath("");
}

void gnomonDataDictWriterCommand::setDataDict(gnomonDataDictSeries *dataDict)
{
    d->dataDict = dataDict;
}

void gnomonDataDictWriterCommand::setForm(gnomonAbstractDynamicForm *form)
{
    d->dataDict = dynamic_cast<gnomonDataDictSeries *>(form);
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonDataDictWriterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["dataDict"] = d->dataDict;
    return inputs;
}

bool gnomonDataDictWriterCommand::isEmpty()
{
    loadPluginGroup("dataDictWriter");
    return gnomonCore::dataDictWriter::pluginFactory().keys().empty();
}

QStringList gnomonDataDictWriterCommand::extensions(void)
{
    return dynamic_cast<gnomonAbstractDataDictWriter *>(this->action)->extensions();
}

gnomonAbstractCommand::orderedMap gnomonDataDictWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("dataDict", "gnomonDataDict"));
    return input_types;
}

void gnomonDataDictWriterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "dataDict") {
        this->setForm(dynamic_cast<gnomonImageSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

QStringList gnomonDataDictWriterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}
