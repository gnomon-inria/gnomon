#include "gnomonBinaryImageVtkVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonBinaryImage/gnomonAbstractBinaryImageVtkVisualization.h>
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
    this->factory = &gnomonVisualization::binaryImageVtkVisualization::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->visu_name = keys[0];
    }
}

gnomonBinaryImageVtkVisualizationCommand::~gnomonBinaryImageVtkVisualizationCommand()
{
    delete d;
}

void gnomonBinaryImageVtkVisualizationCommand::newVisualization(void)
{
    this->clear();
    auto visu = gnomonVisualization::binaryImageVtkVisualization::pluginFactory().create(this->visu_name);
    this->visu = std::shared_ptr<gnomonAbstractBinaryImageVtkVisualization>(visu);
    this->connectVisualization();
}

void gnomonBinaryImageVtkVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setVisualizationName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractBinaryImageVtkVisualization>(this->visu);
    if (visu) {
        visu->setBinaryImage(d->binaryImage);
        this->setVisualizationParameters(parameters);
        visu->refreshParameters();
        visu->update();
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
