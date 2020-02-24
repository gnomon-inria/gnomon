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

#include "gnomonPointCloudWriterCommand.h"
#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonPointCloudWriterCommandPrivate
{
public:
    QString path;
    gnomonPointCloudSeries* pointCloud = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonPointCloudWriterCommand::gnomonPointCloudWriterCommand(const QString& key) : d(new gnomonPointCloudWriterCommandPrivate)
{
    loadPluginGroup("pointCloudWriter");

    this->action = gnomonCore::pointCloudWriter::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonPointCloudWriterCommand::~gnomonPointCloudWriterCommand()
{
    delete d;
}

void gnomonPointCloudWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractPointCloudWriter *) this->action)->setPath(d->path);
    ((gnomonAbstractPointCloudWriter *) this->action)->setPointCloud(d->pointCloud);
    this->action->run();
}

void gnomonPointCloudWriterCommand::undo(void)
{
    ((gnomonAbstractPointCloudWriter *) this->action)->setPath("");
}

void gnomonPointCloudWriterCommand::setPath(const QString& path)
{
    d->path = path;
}

void gnomonPointCloudWriterCommand::setPointCloud(gnomonPointCloudSeries *pointCloud)
{
    d->pointCloud = pointCloud;
}

//
// gnomonPointCloudWriterCommand.cpp ends here
