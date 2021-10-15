// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

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

gnomonImageReaderCommand::gnomonImageReaderCommand(void) : d(new gnomonImageReaderCommandPrivate)
{
    this->factory_name = "imageReader";

    loadPluginGroup(this->factoryName());

    for (auto key: gnomonCore::imageReader::pluginFactory().keys()) {

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

void gnomonImageReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractImageReader *) this->action)->setPath(this->m_path);
    this->action->run();
    gnomonImageSeries *image = ((gnomonAbstractImageReader *) this->action)->image();
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        d->image = nullptr;
    } else {
        d->image = image;
    }
}

void gnomonImageReaderCommand::undo(void)
{
    ((gnomonAbstractImageReader *) this->action)->setPath("");
}

void gnomonImageReaderCommand::setPath(const QString& path)
{
    this->m_path = path;
}

gnomonImageSeries *gnomonImageReaderCommand::image(void)
{
    return d->image;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonImageReaderCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["image"] = this->image();
    return outputs;
}

bool gnomonImageReaderCommand::isEmpty(void)
{
    loadPluginGroup("imageReader");
    return gnomonCore::imageReader::pluginFactory().keys().size() == 0;
}

//
// gnomonImageReaderCommand.cpp ends here
