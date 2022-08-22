#include "gnomonLStringReaderCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringReader.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonLStringReaderCommandPrivate
{
public:
    std::shared_ptr<gnomonLStringSeries> lString = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonLStringReaderCommand::gnomonLStringReaderCommand() : d(new gnomonLStringReaderCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    for (const auto& key: gnomonCore::lStringReader::pluginFactory().keys()) {
        auto algo = gnomonCore::lStringReader::pluginFactory().create(key);
        if (!this->action) {
            this->action = algo;
            this->algorithm_name = key;
        }
        m_descriptions.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
    }
}

gnomonLStringReaderCommand::~gnomonLStringReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonLStringReaderCommand::predo(void)
{
    ((gnomonAbstractLStringReader *) this->action)->setPath(this->m_path);
}

void gnomonLStringReaderCommand::postdo(void)
{
    std::shared_ptr<gnomonLStringSeries> lString = ((gnomonAbstractLStringReader *) this->action)->lString();

    if ((!lString)||(lString->times().empty())) {
        d->lString = nullptr;
    } else {
        d->lString = lString;
    }
}

void gnomonLStringReaderCommand::undo()
{
    ((gnomonAbstractLStringReader *) this->action)->setPath("");
}

std::shared_ptr<gnomonLStringSeries> gnomonLStringReaderCommand::lString()
{
    return d->lString;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringReaderCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["lString"] = this->lString();
    return outputs;
}

bool gnomonLStringReaderCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonLStringReaderCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonLStringReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("lString", "gnomonLString"));
    return types;
}

void gnomonLStringReaderCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->lString) {
        d->lString = std::make_shared<gnomonLStringSeries>();
    }
    auto tmp = serialization["lString"].toObject();
    d->lString->deserialize(tmp);
}

QJsonObject gnomonLStringReaderCommand::serializeResults(void) {
    QJsonObject out;
    out["lString"] = d->lString->serialize();
    return out;
}

//
// gnomonLStringReaderCommand.cpp ends here
