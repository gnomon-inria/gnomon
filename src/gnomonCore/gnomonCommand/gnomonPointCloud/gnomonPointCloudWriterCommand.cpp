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

#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudWriter.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

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

gnomonPointCloudWriterCommand::gnomonPointCloudWriterCommand() : d(new gnomonPointCloudWriterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::pointCloudWriter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::pointCloudWriter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonPointCloudWriterCommand::~gnomonPointCloudWriterCommand()
{
    delete d;
}

void gnomonPointCloudWriterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::pointCloudWriter::pluginFactory().create(algo_name);
}


void gnomonPointCloudWriterCommand::predo(void)
{
    ((gnomonAbstractPointCloudWriter *) this->action)->setPath(this->m_path);
    ((gnomonAbstractPointCloudWriter *) this->action)->setPointCloud(d->pointCloud);
}

void gnomonPointCloudWriterCommand::postdo(void)
{

}

void gnomonPointCloudWriterCommand::undo()
{
    ((gnomonAbstractPointCloudWriter *) this->action)->setPath("");
}

void gnomonPointCloudWriterCommand::setPointCloud(gnomonPointCloudSeries *pointCloud)
{
    d->pointCloud = pointCloud;
}

void gnomonPointCloudWriterCommand::setForm(gnomonAbstractDynamicForm *form)
{
    d->pointCloud = dynamic_cast<gnomonPointCloudSeries*>(form);
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonPointCloudWriterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["pointCloud"] = d->pointCloud;
    return inputs;
}

bool gnomonPointCloudWriterCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonPointCloudWriterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonPointCloudWriterCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("pointCloud", "gnomonPointCloud"));
    return input_types;
}

void gnomonPointCloudWriterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "pointCloud") {
        this->setPointCloud(dynamic_cast<gnomonPointCloudSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}
//
// gnomonPointCloudWriterCommand.cpp ends here
