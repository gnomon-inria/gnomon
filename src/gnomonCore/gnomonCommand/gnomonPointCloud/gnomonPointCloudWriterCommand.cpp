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
    gnomonPointCloudSeries* pointCloud = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonPointCloudWriterCommand::gnomonPointCloudWriterCommand(void) : d(new gnomonPointCloudWriterCommandPrivate)
{
    this->factory_name = "pointCloudWriter";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::meshFromImage::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::meshFromImage::pluginFactory().create(this->algorithm_name);
    }
}

gnomonPointCloudWriterCommand::~gnomonPointCloudWriterCommand()
{
    delete d;
}

void gnomonPointCloudWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::pointCloudWriter::pluginFactory().create(algo_name);
}


void gnomonPointCloudWriterCommand::redo(void)
{
    Q_ASSERT(this->action);
    ((gnomonAbstractPointCloudWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractPointCloudWriter *) this->action)->setPointCloud(d->pointCloud);
    this->action->run();
}

void gnomonPointCloudWriterCommand::undo(void)
{
    ((gnomonAbstractPointCloudWriter *) this->action)->setPath("");
}

void gnomonPointCloudWriterCommand::setPath(const QString& path)
{
    this->m_path = path;
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
