#include "gnomonImageVtkVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonImage/gnomonAbstractImageVtkVisualization.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonImageVtkVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonImageVtkVisualizationCommand::gnomonImageVtkVisualizationCommand() : d(new gnomonImageVtkVisualizationCommandPrivate)
{
    this->factory_name = groupName;
    this->factory = &gnomonVisualization::imageVtkVisualization::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
    }
}

gnomonImageVtkVisualizationCommand::~gnomonImageVtkVisualizationCommand()
{
    delete d;
}

void gnomonImageVtkVisualizationCommand::setAlgorithmName(const QString& visu_name)
{
    if ((this->algorithm_name != visu_name) || (this->visu == nullptr)) {
        gnomonAbstractVtkVisualizationCommand::setAlgorithmName(visu_name);
        auto visu = gnomonVisualization::imageVtkVisualization::pluginFactory().create(visu_name);
        this->visu = std::shared_ptr<gnomonAbstractImageVtkVisualization>(visu);
        this->connectVisualization();
    }
}

void gnomonImageVtkVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setAlgorithmName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractImageVtkVisualization>(this->visu);
    if (visu) {
        if (visu->image() != d->image) {
            visu->setImage(d->image);
            this->setVisualizationParameters(parameters);
            visu->update();
        }
    }
}

void gnomonImageVtkVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->image = std::dynamic_pointer_cast<gnomonImageSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonImageVtkVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["image"] = d->image;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonImageVtkVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("image", "gnomonImage"));
    return input_types;
}

void gnomonImageVtkVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "image") {
        auto image = std::dynamic_pointer_cast<gnomonImageSeries>(form);
        d->image = image;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonImageVtkVisualizationCommand.cpp ends here
