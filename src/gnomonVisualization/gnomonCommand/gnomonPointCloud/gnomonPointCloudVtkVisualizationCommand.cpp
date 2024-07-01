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
        this->visu_name = keys[0];
    }
}

gnomonPointCloudVtkVisualizationCommand::~gnomonPointCloudVtkVisualizationCommand()
{
    delete d;
}

void gnomonPointCloudVtkVisualizationCommand::newVisualization(void)
{
    gnomonAbstractVisualizationCommand::clear();
    auto visu = gnomonVisualization::pointCloudVtkVisualization::pluginFactory().create(this->visu_name);
    this->visu = std::shared_ptr<gnomonAbstractPointCloudVtkVisualization>(visu);
    this->connectVisualization();
}

void gnomonPointCloudVtkVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setVisualizationName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractPointCloudVtkVisualization>(this->visu);
    if (visu) {
        if (d->pointCloud) {
            d->pointCloud->load();
        }
        visu->setPointCloud(d->pointCloud);
        this->setVisualizationParameters(parameters);
        visu->refreshParameters();
        visu->update();
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

void gnomonPointCloudVtkVisualizationCommand::clear(void) {
    gnomonAbstractVisualizationCommand::clear();
    d->pointCloud = nullptr;
}

void gnomonPointCloudVtkVisualizationCommand::clear(bool clear_visu) {
    gnomonAbstractVisualizationCommand::clear(clear_visu);
    d->pointCloud = nullptr;
}

//
// gnomonPointCloudVtkVisualizationCommand.cpp ends here
