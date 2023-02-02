#include "gnomonDataDictReaderCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonDataDict/gnomonAbstractDataDictReader.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonDataDictReaderCommandPrivate
{
public:
    std::shared_ptr<gnomonDataDictSeries> dataDict = nullptr;
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
        m_preview.insert(key, algo->preview());
    }
}

gnomonDataDictReaderCommand::~gnomonDataDictReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonDataDictReaderCommand::predo(void)
{
    this->action->is_async = true;
    ((gnomonAbstractDataDictReader *) this->action)->setPath(this->m_path);
}

void gnomonDataDictReaderCommand::postdo(void)
{
    std::shared_ptr<gnomonDataDictSeries> dataDict = ((gnomonAbstractDataDictReader *) this->action)->dataDict();
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

std::shared_ptr<gnomonDataDictSeries> gnomonDataDictReaderCommand::dataDict()
{
    return d->dataDict;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonDataDictReaderCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
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

void gnomonDataDictReaderCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->dataDict) {
        d->dataDict = std::make_shared<gnomonDataDictSeries>();
    }
    auto tmp = serialization["dataDict"].toObject();
    d->dataDict->deserialize(tmp);
}

QJsonObject gnomonDataDictReaderCommand::serializeResults(void) {
    QJsonObject out;
    out["dataDict"] = d->dataDict->serialize();
    return out;
}
