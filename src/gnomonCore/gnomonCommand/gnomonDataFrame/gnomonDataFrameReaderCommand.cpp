#include "gnomonDataFrameReaderCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonDataFrame/gnomonAbstractDataFrameReader.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonDataFrameReaderCommandPrivate
{
public:
    std::shared_ptr<gnomonDataFrameSeries> dataFrame = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonDataFrameReaderCommand::gnomonDataFrameReaderCommand() : d(new gnomonDataFrameReaderCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    for (const auto& key: gnomonCore::dataFrameReader::pluginFactory().keys()) {
        auto algo = gnomonCore::dataFrameReader::pluginFactory().create(key);
        if (!this->action) {
            this->action = algo;
            this->algorithm_name = key;
        }
        m_descriptions.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
    }
}

gnomonDataFrameReaderCommand::~gnomonDataFrameReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonDataFrameReaderCommand::predo(void)
{
    ((gnomonAbstractDataFrameReader *) this->action)->setPath(this->m_path);
}

void gnomonDataFrameReaderCommand::postdo(void)
{
    std::shared_ptr<gnomonDataFrameSeries> dataFrame = ((gnomonAbstractDataFrameReader *) this->action)->dataFrame();

    if ((!dataFrame)||(dataFrame->times().empty())) {
        d->dataFrame = nullptr;
    } else {
        d->dataFrame = dataFrame;
    }
}

void gnomonDataFrameReaderCommand::undo()
{
    ((gnomonAbstractDataFrameReader *) this->action)->setPath("");
}

std::shared_ptr<gnomonDataFrameSeries> gnomonDataFrameReaderCommand::dataFrame()
{
    return d->dataFrame;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonDataFrameReaderCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["dataFrame"] = this->dataFrame();
    return outputs;
}

bool gnomonDataFrameReaderCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonDataFrameReaderCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonDataFrameReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("dataFrame", "gnomonDataFrame"));
    return types;
}

void gnomonDataFrameReaderCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->dataFrame) {
        d->dataFrame = std::make_shared<gnomonDataFrameSeries>();
    }
    auto tmp = serialization["dataFrame"].toObject();
    d->dataFrame->deserialize(tmp);
}

QJsonObject gnomonDataFrameReaderCommand::serializeResults(void) {
    QJsonObject out;
    out["dataFrame"] = d->dataFrame->serialize();
    return out;
}

//
// gnomonDataFrameReaderCommand.cpp ends here
