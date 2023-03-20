#include "gnomonCellImageVtkVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonCellImage/gnomonAbstractCellImageVtkVisualization.h>
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
    this->factory = &gnomonVisualization::cellImageVtkVisualization::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        auto visu = gnomonVisualization::cellImageVtkVisualization::pluginFactory().create(this->algorithm_name);
        this->visu = std::shared_ptr<gnomonAbstractCellImageVtkVisualization>(visu);
    }
}

gnomonCellImageVtkVisualizationCommand::~gnomonCellImageVtkVisualizationCommand()
{
    delete d;
}

void gnomonCellImageVtkVisualizationCommand::setAlgorithmName(const QString& visu_name)
{
    if (this->algorithm_name != visu_name) {
        gnomonAbstractVtkVisualizationCommand::setAlgorithmName(visu_name);
        auto visu = gnomonVisualization::cellImageVtkVisualization::pluginFactory().create(visu_name);
        this->visu = std::shared_ptr<gnomonAbstractCellImageVtkVisualization>(visu);
    }
}

void gnomonCellImageVtkVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setAlgorithmName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractCellImageVtkVisualization>(this->visu);
    if (visu) {
        if (visu->cellImage() != d->cellImage) {
            visu->setCellImage(d->cellImage);
            this->setVisualizationParameters(parameters);
        }
    }
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
