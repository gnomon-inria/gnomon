#include "gnomonCellComplexReaderCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexReader.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonCellComplexReaderCommandPrivate
{
public:
    std::shared_ptr<gnomonCellComplexSeries> cellComplex = nullptr;
};

gnomonCellComplexReaderCommand::gnomonCellComplexReaderCommand() : d(new gnomonCellComplexReaderCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

     for (const auto& key: gnomonCore::cellComplexReader::pluginFactory().keys()) {
        auto algo = gnomonCore::cellComplexReader::pluginFactory().create(key);
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

gnomonCellComplexReaderCommand::~gnomonCellComplexReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonCellComplexReaderCommand::predo(void)
{
    this->action->is_async = true;
    ((gnomonAbstractCellComplexReader *) this->action)->setPath(this->m_path);
}

void gnomonCellComplexReaderCommand::postdo(void)
{
    std::shared_ptr<gnomonCellComplexSeries> cellComplex = ((gnomonAbstractCellComplexReader *) this->action)->cellComplex();

    if((!cellComplex)||(cellComplex->times().empty())) {
        d->cellComplex = nullptr;
    } else {
        d->cellComplex = cellComplex;
    }
}

void gnomonCellComplexReaderCommand::undo()
{
    ((gnomonAbstractCellComplexReader *) this->action)->setPath("");
}

std::shared_ptr<gnomonCellComplexSeries> gnomonCellComplexReaderCommand::cellComplex()
{
    return d->cellComplex;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellComplexReaderCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["cellComplex"] = this->cellComplex();
    return outputs;
}

bool gnomonCellComplexReaderCommand::isEmpty()
{
    return availablePlugins().empty();
}

gnomonAbstractCommand::orderedMap gnomonCellComplexReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("cellComplex", "gnomonCellComplex"));
    return types;
}

QStringList gnomonCellComplexReaderCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

void gnomonCellComplexReaderCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->cellComplex) {
        d->cellComplex = std::make_shared<gnomonCellComplexSeries>();
    }
    auto tmp = serialization["cellComplex"].toObject();
    d->cellComplex->deserialize(tmp);
}

QJsonObject gnomonCellComplexReaderCommand::serializeResults(void) {
    QJsonObject out;
    out["cellComplex"] = d->cellComplex->serialize();
    return out;
}

//
// gnomonCellComplexReaderCommand.cpp ends here
