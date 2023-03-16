#include "gnomonCellImageVtkVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonCellImage/gnomonAbstractVisualizationCellImage.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonCellImageVtkVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonCellImageSeries> cellImage = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageVtkVisualizationCommand::gnomonCellImageVtkVisualizationCommand() : d(new gnomonCellImageVtkVisualizationCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonVisualization::visualizationCellImage::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        auto visu = gnomonVisualization::visualizationCellImage::pluginFactory().create(this->algorithm_name);
        this->visu = std::shared_ptr<gnomonAbstractVisualizationCellImage>(visu);
    }
}

gnomonCellImageVtkVisualizationCommand::~gnomonCellImageVtkVisualizationCommand()
{
    delete d;
}

void gnomonCellImageVtkVisualizationCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    this->visu->deleteLater();
    auto visu = gnomonVisualization::visualizationCellImage::pluginFactory().create(algo_name);
    this->visu = std::shared_ptr<gnomonAbstractVisualizationCellImage>(visu);
}

void gnomonCellImageVtkVisualizationCommand::predo(void)
{
    std::dynamic_pointer_cast<gnomonAbstractVisualizationCellImage>(this->visu)->setCellImage(d->cellImage);
}

void gnomonCellImageVtkVisualizationCommand::postdo(void)
{

}

void gnomonCellImageVtkVisualizationCommand::undo()
{
}

void gnomonCellImageVtkVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->cellImage = std::dynamic_pointer_cast<gnomonCellImageSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageVtkVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["cellImage"] = d->cellImage;
    return inputs;
}

bool gnomonCellImageVtkVisualizationCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonCellImageVtkVisualizationCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonCellImageVtkVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("cellImage", "gnomonCellImage"));
    return input_types;
}

void gnomonCellImageVtkVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "cellImage") {
        auto cellImage = std::dynamic_pointer_cast<gnomonCellImageSeries>(form);
        d->cellImage = cellImage;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonCellImageVtkVisualizationCommand.cpp ends here
