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
    this->factory_name = "pointCloudWriter";
    loadPluginGroup(this->factoryName());

    this->algorithm_name = key;
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

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudWriterCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["pointCloud"] = d->pointCloud;
    return inputs;
}

bool gnomonPointCloudWriterCommand::isEmpty(void)
{
    loadPluginGroup("pointCloudWriter");
    return gnomonCore::pointCloudWriter::pluginFactory().keys().size() == 0;
}
//
// gnomonPointCloudWriterCommand.cpp ends here
