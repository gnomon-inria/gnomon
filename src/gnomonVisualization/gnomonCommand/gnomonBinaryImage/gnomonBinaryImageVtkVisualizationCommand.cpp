#include "gnomonBinaryImageVtkVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonBinaryImage/gnomonAbstractVisualizationBinaryImage.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonBinaryImageVtkVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonBinaryImageSeries> binaryImage = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonBinaryImageVtkVisualizationCommand::gnomonBinaryImageVtkVisualizationCommand() : d(new gnomonBinaryImageVtkVisualizationCommandPrivate)
{
    this->factory_name = groupName;
    this->factory = &gnomonVisualization::visualizationBinaryImage::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        auto visu = gnomonVisualization::visualizationBinaryImage::pluginFactory().create(this->algorithm_name);
        this->visu = std::shared_ptr<gnomonAbstractVisualizationBinaryImage>(visu);
    }
}

gnomonBinaryImageVtkVisualizationCommand::~gnomonBinaryImageVtkVisualizationCommand()
{
    delete d;
}

void gnomonBinaryImageVtkVisualizationCommand::setAlgorithmName(const QString& visu_name)
{
    if (this->algorithm_name != visu_name) {
        gnomonAbstractVtkVisualizationCommand::setAlgorithmName(visu_name);
        auto visu = gnomonVisualization::visualizationBinaryImage::pluginFactory().create(visu_name);
        this->visu = std::shared_ptr<gnomonAbstractVisualizationBinaryImage>(visu);
    }
}

void gnomonBinaryImageVtkVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setAlgorithmName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractVisualizationBinaryImage>(this->visu);
    if (visu) {
        if (visu->binaryImage() != d->binaryImage) {
            visu->setBinaryImage(d->binaryImage);
            this->setVisualizationParameters(parameters);
        }
    }
}

void gnomonBinaryImageVtkVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->binaryImage = std::dynamic_pointer_cast<gnomonBinaryImageSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonBinaryImageVtkVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["binaryImage"] = d->binaryImage;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonBinaryImageVtkVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("binaryImage", "gnomonBinaryImage"));
    return input_types;
}

void gnomonBinaryImageVtkVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "binaryImage") {
        auto binaryImage = std::dynamic_pointer_cast<gnomonBinaryImageSeries>(form);
        d->binaryImage = binaryImage;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonBinaryImageVtkVisualizationCommand.cpp ends here
