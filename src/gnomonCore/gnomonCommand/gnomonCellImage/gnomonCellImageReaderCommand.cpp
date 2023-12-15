#include "gnomonCellImageReaderCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageReader.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonCellImageReaderCommandPrivate
{
public:
    std::shared_ptr<gnomonCellImageSeries> cellImage = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageReaderCommand::gnomonCellImageReaderCommand() : d(new gnomonCellImageReaderCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonCellImageReaderCommand)

    for (const auto& key: gnomonCore::cellImageReader::pluginFactory().keys()) {
        auto algo = gnomonCore::cellImageReader::pluginFactory().create(key);
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

gnomonCellImageReaderCommand::~gnomonCellImageReaderCommand()
{
    this->action = nullptr;

    delete d;
}

void gnomonCellImageReaderCommand::predo(void)
{
    this->action->is_async = true;
    ((gnomonAbstractCellImageReader *) this->action)->setPath(this->m_path);
}

void gnomonCellImageReaderCommand::postdo(void)
{
    std::shared_ptr<gnomonCellImageSeries> cellImage = ((gnomonAbstractCellImageReader *) this->action)->cellImage();

    if ((!cellImage)||(cellImage->times().empty())) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }
}

void gnomonCellImageReaderCommand::undo()
{
    ((gnomonAbstractCellImageReader *) this->action)->setPath("");
}

std::shared_ptr<gnomonCellImageSeries> gnomonCellImageReaderCommand::cellImage()
{
    return d->cellImage;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageReaderCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["cellImage"] = this->cellImage();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonCellImageReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    return types;
}

void gnomonCellImageReaderCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->cellImage) {
        d->cellImage = std::make_shared<gnomonCellImageSeries>();
    }
    auto tmp = serialization["cellImage"].toObject();
    d->cellImage->deserialize(tmp);
}

QJsonObject gnomonCellImageReaderCommand::serializeResults(void) {
    QJsonObject out;
    out["cellImage"] = d->cellImage->serialize();
    return out;
}

GNOMON_REGISTER_TYPE(gnomonCellImageReaderCommand)
//
// gnomonCellImageReaderCommand.cpp ends here
