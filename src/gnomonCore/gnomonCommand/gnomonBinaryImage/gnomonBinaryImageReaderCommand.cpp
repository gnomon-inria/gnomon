#include "gnomonBinaryImageReaderCommand.h"

class gnomonBinaryImageReaderCommandPrivate
{
public:
    gnomonBinaryImageSeries *binaryImage = nullptr;
};

gnomonBinaryImageReaderCommand::gnomonBinaryImageReaderCommand() : d(new gnomonBinaryImageReaderCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    for (const auto& key: gnomonCore::binaryImageReader::pluginFactory().keys()) {
        auto algo = gnomonCore::binaryImageReader::pluginFactory().create(key);
        if (!this->action) {
            this->action = algo;
            this->algorithm_name = key;
        }
        m_descriptions.insert(key, algo->documentation());
        m_extensions.insert(key, algo->extensions());
        m_actions.insert(key, algo);
    }
}

gnomonBinaryImageReaderCommand::~gnomonBinaryImageReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonBinaryImageReaderCommand::predo(void)
{
    ((gnomonAbstractBinaryImageReader *) this->action)->setPath(this->m_path);
}

void gnomonBinaryImageReaderCommand::postdo(void)
{
    gnomonBinaryImageSeries *binaryImage = ((gnomonAbstractBinaryImageReader *) this->action)->binaryImage();

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

gnomonBinaryImageSeries *gnomonBinaryImageReaderCommand::binaryImage()
{
    return d->binaryImage;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonBinaryImageReaderCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["binaryImage"] = this->binaryImage();
    return outputs;
}

bool gnomonBinaryImageReaderCommand::isEmpty()
{
    return availablePlugins().empty();
}

gnomonAbstractCommand::orderedMap gnomonBinaryImageReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("binaryImage", "gnomonBinaryImage"));
    return types;
}

QStringList gnomonBinaryImageReaderCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

void gnomonBinaryImageReaderCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->binaryImage) {
        d->binaryImage = new gnomonBinaryImageSeries();
    }
    auto tmp = serialization["binaryImage"].toObject();
    d->binaryImage->deserialize(tmp);
}

QJsonObject gnomonBinaryImageReaderCommand::serializeResults(void) {
    QJsonObject out;
    out["binaryImage"] = d->binaryImage->serialize();
    return out;
}
