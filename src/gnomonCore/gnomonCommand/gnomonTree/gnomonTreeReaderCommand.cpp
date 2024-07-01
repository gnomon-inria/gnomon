#include "gnomonTreeReaderCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeReader.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonTreeReaderCommandPrivate
{
public:
    std::shared_ptr<gnomonTreeSeries> tree = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeReaderCommand::gnomonTreeReaderCommand() : d(new gnomonTreeReaderCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonTreeReaderCommand)

    for (const auto& key: gnomonCore::treeReader::pluginFactory().keys()) {
        auto algo = gnomonCore::treeReader::pluginFactory().create(key);
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

gnomonTreeReaderCommand::~gnomonTreeReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonTreeReaderCommand::predo(void)
{
    this->action->is_async = true;
    ((gnomonAbstractTreeReader *) this->action)->setPath(this->m_path);
}

void gnomonTreeReaderCommand::postdo(void)
{
    std::shared_ptr<gnomonTreeSeries> tree = ((gnomonAbstractTreeReader *) this->action)->tree();

    if ((!tree)||(tree->times().empty())) {
        d->tree = nullptr;
    } else {
        d->tree = tree;
    }
}

void gnomonTreeReaderCommand::undo()
{
    ((gnomonAbstractTreeReader *) this->action)->setPath("");
}

std::shared_ptr<gnomonTreeSeries> gnomonTreeReaderCommand::tree()
{
    return d->tree;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonTreeReaderCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["tree"] = this->tree();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonTreeReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("tree", "gnomonTree"));
    return types;
}

void gnomonTreeReaderCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->tree) {
        d->tree = std::make_shared<gnomonTreeSeries>();
    }
    auto tmp = serialization["tree"].toObject();
    d->tree->deserialize(tmp);
}

QJsonObject gnomonTreeReaderCommand::serializeResults(void) {
    QJsonObject out;
    out["tree"] = d->tree->serialize();
    return out;
}

void gnomonTreeReaderCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->tree = nullptr;
}

GNOMON_REGISTER_TYPE(gnomonTreeReaderCommand)

//
// gnomonTreeReaderCommand.cpp ends here
