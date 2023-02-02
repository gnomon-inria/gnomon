#include "gnomonImageReaderCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonImage/gnomonAbstractImageReader.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageReaderCommandPrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageReaderCommand::gnomonImageReaderCommand() : d(new gnomonImageReaderCommandPrivate)
{
    this->factory_name = groupName;

    loadPluginGroup(this->factoryName());

    for (const auto& key: gnomonCore::imageReader::pluginFactory().keys()) {

        auto algo = gnomonCore::imageReader::pluginFactory().create(key);

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

gnomonImageReaderCommand::~gnomonImageReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonImageReaderCommand::predo(void)
{
    this->action->is_async = true;
    ((gnomonAbstractImageReader *) this->action)->setPath(this->m_path);
}

void gnomonImageReaderCommand::postdo(void)
{
    std::shared_ptr<gnomonImageSeries> image = ((gnomonAbstractImageReader *) this->action)->image();

    if ((!image)||(image->times().empty())||(image->current()->channels().empty())) {
        d->image = nullptr;
        QString str = "pb reading Image " + this->m_path;
        if(!image) str += "  image is empty";
        else if(image->times().empty()) str += "  no times available";
        else str += "  no channels availables";
        
        dtkWarn() << Q_FUNC_INFO << str;
    } else {
        d->image = image;
    }
}

void gnomonImageReaderCommand::undo()
{
    ((gnomonAbstractImageReader *) this->action)->setPath("");
}

std::shared_ptr<gnomonImageSeries> gnomonImageReaderCommand::image()
{
    return d->image;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonImageReaderCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["image"] = this->image();
    return outputs;
}

bool gnomonImageReaderCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonImageReaderCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonImageReaderCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("image", "gnomonImage"));
    return types;
}

void gnomonImageReaderCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->image) {
        d->image = std::make_shared<gnomonImageSeries>();
    }
    auto tmp = serialization["image"].toObject();
    d->image->deserialize(tmp);
}

QJsonObject gnomonImageReaderCommand::serializeResults(void) {
    QJsonObject out;
    out["image"] = d->image->serialize();
    return out;
}

//
// gnomonImageReaderCommand.cpp ends here
