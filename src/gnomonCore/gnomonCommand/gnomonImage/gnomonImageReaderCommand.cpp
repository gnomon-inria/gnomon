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
    gnomonImageSeries *image = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageReaderCommand::gnomonImageReaderCommand(const QString& key) : d(new gnomonImageReaderCommandPrivate)
{
    loadPluginGroup(this->factoryName());

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
    d->path = path;
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


const QString gnomonImageReaderCommand::factoryName(void)
{
    return "imageReader";
}

bool gnomonImageReaderCommand::isEmpty(void)
{
    loadPluginGroup(gnomonImageReaderCommand::factoryName());
    return gnomonCore::imageReader::pluginFactory().keys().size() == 0;
}

//
// gnomonImageReaderCommand.cpp ends here
