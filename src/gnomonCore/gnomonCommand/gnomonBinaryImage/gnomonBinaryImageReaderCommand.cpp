#include "gnomonBinaryImageReaderCommand.h"

class gnomonBinaryImageReaderCommandPrivate
{
public:
    gnomonBinaryImageSeries *binaryImage = nullptr;
};

gnomonBinaryImageReaderCommand::gnomonBinaryImageReaderCommand(void) : d(new gnomonBinaryImageReaderCommandPrivate)
{
    this->factory_name = "binaryImageReader";
    loadPluginGroup(this->factoryName());

    for (auto key: gnomonCore::binaryImageReader::pluginFactory().keys()) {
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

void gnomonBinaryImageReaderCommand::redo(void)
{
    Q_ASSERT(this->action);

    ((gnomonAbstractBinaryImageReader *) this->action)->setPath(this->m_path);
    this->action->run();
    gnomonBinaryImageSeries *binaryImage = ((gnomonAbstractBinaryImageReader *) this->action)->binaryImage();
    if ((!binaryImage)||(binaryImage->times().size()==0)) {
        d->binaryImage = nullptr;
    } else {
        d->binaryImage = binaryImage;
    }
}

void gnomonBinaryImageReaderCommand::undo(void)
{
    ((gnomonAbstractBinaryImageReader *) this->action)->setPath("");
}

void gnomonBinaryImageReaderCommand::setPath(const QString& path)
{
    this->m_path = path;
}

gnomonBinaryImageSeries *gnomonBinaryImageReaderCommand::binaryImage(void)
{
    return d->binaryImage;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonBinaryImageReaderCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["binaryImage"] = this->binaryImage();
    return outputs;
}

bool gnomonBinaryImageReaderCommand::isEmpty(void)
{
    loadPluginGroup("binaryImageReader");
    return gnomonCore::binaryImageReader::pluginFactory().keys().size() == 0;
}
