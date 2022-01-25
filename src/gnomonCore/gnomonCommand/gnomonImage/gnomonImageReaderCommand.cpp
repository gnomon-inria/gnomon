#include "gnomonImageReaderCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageReaderCommandPrivate
{
public:
    gnomonImageSeries *image = nullptr;
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
    }
}

gnomonImageReaderCommand::~gnomonImageReaderCommand()
{
    this->action = nullptr;
    delete d;
}

void gnomonImageReaderCommand::predo(void)
{
    qWarning() << Q_FUNC_INFO;

    ((gnomonAbstractImageReader *) this->action)->setPath(this->m_path);

    qWarning() << Q_FUNC_INFO << "Done";
}

void gnomonImageReaderCommand::postdo(void)
{
    qWarning() << Q_FUNC_INFO;

    gnomonImageSeries *image = ((gnomonAbstractImageReader *) this->action)->image();

    qWarning() << Q_FUNC_INFO << "Data" << image;

    if ((!image)||(image->times().empty())||(((gnomonImage *)image->current())->channels().empty())) {
        d->image = nullptr;
    } else {
        d->image = image;
    }

    qWarning() << Q_FUNC_INFO << "Done" << this->image();
}

void gnomonImageReaderCommand::undo()
{
    ((gnomonAbstractImageReader *) this->action)->setPath("");
}

gnomonImageSeries *gnomonImageReaderCommand::image()
{
    return d->image;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageReaderCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
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

//
// gnomonImageReaderCommand.cpp ends here
