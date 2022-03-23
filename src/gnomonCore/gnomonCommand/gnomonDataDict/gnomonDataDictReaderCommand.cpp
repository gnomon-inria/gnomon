#include "gnomonDataDictReaderCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonDataDict/gnomonAbstractDataDictReader.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonDataDictReaderCommandPrivate
{
public:
    gnomonDataDictSeries *dataDict = nullptr;
};

gnomonDataDictReaderCommand::gnomonDataDictReaderCommand() : d(new gnomonDataDictReaderCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    for (const auto& key: gnomonCore::dataDictReader::pluginFactory().keys()) {
        auto algo = gnomonCore::dataDictReader::pluginFactory().create(key);
        if (!this->action) {
            this->action = algo;
            this->algorithm_name = key;
        }
        m_descriptions.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
    }
}

gnomonDataDictReaderCommand::~gnomonDataDictReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonDataDictReaderCommand::predo(void)
{
    ((gnomonAbstractDataDictReader *) this->action)->setPath(this->m_path);
}

void gnomonDataDictReaderCommand::postdo(void)
{
    gnomonDataDictSeries *dataDict = ((gnomonAbstractDataDictReader *) this->action)->dataDict();
    if ((!dataDict)||(dataDict->times().empty())) {
        d->dataDict = nullptr;
    } else {
        d->dataDict = dataDict;
    }
}

void gnomonDataDictReaderCommand::undo()
{
    ((gnomonAbstractDataDictReader *) this->action)->setPath("");
}

gnomonDataDictSeries *gnomonDataDictReaderCommand::dataDict()
{
    return d->dataDict;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonDataDictReaderCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["dataDict"] = this->dataDict();
    return outputs;
}

bool gnomonDataDictReaderCommand::isEmpty()
{
    loadPluginGroup("dataDictReader");
    return gnomonCore::dataDictReader::pluginFactory().keys().empty();
}

gnomonAbstractCommand::orderedMap gnomonDataDictReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("dataDict", "gnomonDataDict"));
    return types;
}

QStringList gnomonDataDictReaderCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}
