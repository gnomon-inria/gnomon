#include "gnomonPointCloudVtkVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonPointCloud/gnomonAbstractPointCloudVtkVisualization.h>
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
    this->factory = &gnomonVisualization::pointCloudVtkVisualization::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
    }
}

gnomonPointCloudVtkVisualizationCommand::~gnomonPointCloudVtkVisualizationCommand()
{
    delete d;
}

void gnomonPointCloudVtkVisualizationCommand::setAlgorithmName(const QString& visu_name)
{
    if ((this->algorithm_name != visu_name) || (this->visu == nullptr)) {
        gnomonAbstractVtkVisualizationCommand::setAlgorithmName(visu_name);
        auto visu = gnomonVisualization::pointCloudVtkVisualization::pluginFactory().create(visu_name);
        this->visu = std::shared_ptr<gnomonAbstractPointCloudVtkVisualization>(visu);
        this->connectVisualization();
    }
}

void gnomonPointCloudVtkVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setAlgorithmName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractPointCloudVtkVisualization>(this->visu);
    if (visu) {
        if (visu->pointCloud() != d->pointCloud) {
            visu->setPointCloud(d->pointCloud);
            visu->refreshParameters();
            this->setVisualizationParameters(parameters);
            visu->update();
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
