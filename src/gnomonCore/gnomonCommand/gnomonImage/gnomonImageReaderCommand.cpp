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
    QString path;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageReaderCommand::gnomonImageReaderCommand(const QString& key) : d(new gnomonImageReaderCommandPrivate)
{
    loadPluginGroup("imageReader");

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
    ((gnomonAbstractImageReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonImageReaderCommand::undo(void)
{
    ((gnomonAbstractImageReader *) this->action)->setPath("");
}

void gnomonImageReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonImageSeries *gnomonImageReaderCommand::image(void)
{
    gnomonImageSeries *image = ((gnomonAbstractImageReader *) this->action)->image();
    if ((!image)||(image->times().size()==0)||(((gnomonImage *)image->current())->channels().size()==0)) {
        return nullptr;
    } else {
        return image;
    }
}

bool gnomonImageReaderCommand::isEmpty(void)
{
    loadPluginGroup("imageReader");
    return gnomonCore::imageReader::pluginFactory().keys().size() == 0;
}

//
// gnomonImageReaderCommand.cpp ends here
