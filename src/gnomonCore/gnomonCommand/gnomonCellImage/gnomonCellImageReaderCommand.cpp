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

#include "gnomonCellImageReaderCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonCellImageReaderCommandPrivate
{
public:
    QString path;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageReaderCommand::gnomonCellImageReaderCommand(const QString& key) : d(new gnomonCellImageReaderCommandPrivate)
{
    loadPluginGroup("cellImageReader");

    this->action = gnomonCore::cellImageReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonCellImageReaderCommand::~gnomonCellImageReaderCommand()
{
    delete d;
}

void gnomonCellImageReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractCellImageReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonCellImageReaderCommand::undo(void)
{
    ((gnomonAbstractCellImageReader *) this->action)->setPath("");
}

void gnomonCellImageReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonCellImageSeries *gnomonCellImageReaderCommand::cellImage(void)
{
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageReader *) this->action)->cellImage();
    if ((!cellImage)||(cellImage->times().size()==0)) {
        return nullptr;
    } else {
        return cellImage;
    }
}

bool gnomonCellImageReaderCommand::isEmpty(void)
{
    loadPluginGroup("cellImageReader");
    return gnomonCore::cellImageReader::pluginFactory().keys().size() == 0;
}

//
// gnomonCellImageReaderCommand.cpp ends here
