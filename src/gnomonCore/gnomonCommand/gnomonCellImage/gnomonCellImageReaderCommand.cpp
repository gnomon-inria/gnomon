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
    gnomonCellImageSeries *cellImage;
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
    ((gnomonAbstractCellImageReader *) this->action)->setPath(this->m_path);
    this->action->run();
    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageReader *) this->action)->cellImage();
    if ((!cellImage)||(cellImage->times().size()==0)) {
        d->cellImage = nullptr;
    } else {
        d->cellImage = cellImage;
    }
}

void gnomonCellImageReaderCommand::undo(void)
{
    ((gnomonAbstractCellImageReader *) this->action)->setPath("");
}

void gnomonCellImageReaderCommand::setPath(const QString& path)
{
    this->m_path = path;
}

gnomonCellImageSeries *gnomonCellImageReaderCommand::cellImage(void)
{
    return d->cellImage;
}

bool gnomonCellImageReaderCommand::isEmpty(void)
{
    loadPluginGroup("cellImageReader");
    return gnomonCore::cellImageReader::pluginFactory().keys().size() == 0;
}

//
// gnomonCellImageReaderCommand.cpp ends here
