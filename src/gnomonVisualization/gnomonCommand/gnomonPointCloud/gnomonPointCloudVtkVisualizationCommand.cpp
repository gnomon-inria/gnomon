#include "gnomonPointCloudVtkVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonPointCloud/gnomonAbstractVisualizationPointCloud.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonPointCloudVtkVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonPointCloudSeries> pointCloud = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonPointCloudVtkVisualizationCommand::gnomonPointCloudVtkVisualizationCommand() : d(new gnomonPointCloudVtkVisualizationCommandPrivate)
{
    this->factory_name = groupName;
    this->factory = &gnomonVisualization::visualizationPointCloud::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        auto visu = gnomonVisualization::visualizationPointCloud::pluginFactory().create(this->algorithm_name);
        this->visu = std::shared_ptr<gnomonAbstractVisualizationPointCloud>(visu);
    }
}

gnomonPointCloudVtkVisualizationCommand::~gnomonPointCloudVtkVisualizationCommand()
{
    delete d;
}

void gnomonPointCloudVtkVisualizationCommand::setAlgorithmName(const QString& visu_name)
{
    if (this->algorithm_name != visu_name) {
        gnomonAbstractVtkVisualizationCommand::setAlgorithmName(visu_name);
        auto visu = gnomonVisualization::visualizationPointCloud::pluginFactory().create(visu_name);
        this->visu = std::shared_ptr<gnomonAbstractVisualizationPointCloud>(visu);
    }
}

void gnomonPointCloudVtkVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setAlgorithmName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractVisualizationPointCloud>(this->visu);
    if (visu) {
        if (visu->pointCloud() != d->pointCloud) {
            visu->setPointCloud(d->pointCloud);
            this->setVisualizationParameters(parameters);
        }
    }
}

void gnomonPointCloudVtkVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->pointCloud = std::dynamic_pointer_cast<gnomonPointCloudSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonPointCloudVtkVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["pointCloud"] = d->pointCloud;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonPointCloudVtkVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("pointCloud", "gnomonPointCloud"));
    return input_types;
}

void gnomonPointCloudVtkVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "pointCloud") {
        auto pointCloud = std::dynamic_pointer_cast<gnomonPointCloudSeries>(form);
        d->pointCloud = pointCloud;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonPointCloudVtkVisualizationCommand.cpp ends here
