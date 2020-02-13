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

#include "gnomonPointCloudReaderCommand.h"

#include <dtkScript>

class gnomonPointCloudReaderCommandPrivate
{
public:
    QString path;
};

gnomonPointCloudReaderCommand::gnomonPointCloudReaderCommand(const QString& key) : d(new gnomonPointCloudReaderCommandPrivate)
{
    loadPluginGroup("pointCloudReader");

    this->action = gnomonCore::pointCloudReader::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonPointCloudReaderCommand::~gnomonPointCloudReaderCommand()
{
    delete d;
}

void gnomonPointCloudReaderCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractPointCloudReader *) this->action)->setPath(d->path);
    this->action->run();
}

void gnomonPointCloudReaderCommand::undo(void)
{
    ((gnomonAbstractPointCloudReader *) this->action)->setPath("");
}

void gnomonPointCloudReaderCommand::setPath(const QString& path)
{
    d->path = path;
}

gnomonPointCloudSeries *gnomonPointCloudReaderCommand::pointCloud(void)
{
    gnomonPointCloudSeries *pointCloud = ((gnomonAbstractPointCloudReader *) this->action)->pointCloud();
    if ((!pointCloud)||(pointCloud->times().size()==0)) {
        return nullptr;
    } else {
        return pointCloud;
    }
}

//
// gnomonPointCloudReaderCommand.cpp ends here
