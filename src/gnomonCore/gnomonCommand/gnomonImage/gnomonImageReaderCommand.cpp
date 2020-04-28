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

gnomonImageReaderCommand::gnomonImageReaderCommand(const QString& key) : d(new gnomonImageReaderCommandPrivate)
{
    this->factory_name = "imageReader";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
    this->action = gnomonCore::imageReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonImageReaderCommand::~gnomonImageReaderCommand()
{
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
