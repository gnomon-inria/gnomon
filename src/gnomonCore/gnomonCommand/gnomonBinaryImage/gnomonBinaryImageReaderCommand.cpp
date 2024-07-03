#include "gnomonBinaryImageReaderCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonBinaryImage/gnomonAbstractBinaryImageReader.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class GNOMONCORE_EXPORT gnomonBinaryImageReaderCommandPrivate
{
public:
    std::shared_ptr<gnomonBinaryImageSeries> binaryImage = nullptr;
};

gnomonBinaryImageReaderCommand::gnomonBinaryImageReaderCommand() : d(new gnomonBinaryImageReaderCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonBinaryImageReaderCommand)

    for (const auto& key: gnomonCore::binaryImageReader::pluginFactory().keys()) {
        auto algo = gnomonCore::binaryImageReader::pluginFactory().create(key);
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

gnomonBinaryImageReaderCommand::~gnomonBinaryImageReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonBinaryImageReaderCommand::predo(void)
{
    this->action->is_async = true;
    ((gnomonAbstractBinaryImageReader *) this->action)->setPath(this->m_path);
}

void gnomonBinaryImageReaderCommand::postdo(void)
{
    std::shared_ptr<gnomonBinaryImageSeries> binaryImage = ((gnomonAbstractBinaryImageReader *) this->action)->binaryImage();

    if ((!binaryImage)||(binaryImage->times().empty())) {
        d->binaryImage = nullptr;
    } else {
        d->binaryImage = binaryImage;
    }
}

void gnomonBinaryImageReaderCommand::undo()
{
    ((gnomonAbstractBinaryImageReader *) this->action)->setPath("");
}

std::shared_ptr<gnomonBinaryImageSeries> gnomonBinaryImageReaderCommand::binaryImage()
{
    return d->binaryImage;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonBinaryImageReaderCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> outputs;
    outputs["binaryImage"] = this->binaryImage();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonBinaryImageReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("binaryImage", "gnomonBinaryImage"));
    return types;
}

void gnomonBinaryImageReaderCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->binaryImage) {
        d->binaryImage = std::make_shared<gnomonBinaryImageSeries>();
    }
    auto tmp = serialization["binaryImage"].toObject();
    d->binaryImage->deserialize(tmp);
}

QJsonObject gnomonBinaryImageReaderCommand::serializeResults(void) {
    QJsonObject out;
    out["binaryImage"] = d->binaryImage->serialize();
    return out;
}

void gnomonBinaryImageReaderCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->binaryImage = nullptr;
}

GNOMON_REGISTER_TYPE(gnomonBinaryImageReaderCommand)
