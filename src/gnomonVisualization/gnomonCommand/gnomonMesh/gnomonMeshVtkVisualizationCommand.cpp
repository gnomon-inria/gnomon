#include "gnomonMeshVtkVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonMesh/gnomonAbstractMeshVtkVisualization.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonMeshVtkVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonMeshSeries> mesh = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonMeshVtkVisualizationCommand::gnomonMeshVtkVisualizationCommand() : d(new gnomonMeshVtkVisualizationCommandPrivate)
{
    this->factory_name = groupName;
    this->factory = &gnomonVisualization::meshVtkVisualization::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->visu_name = keys[0];
    }
}

gnomonMeshVtkVisualizationCommand::~gnomonMeshVtkVisualizationCommand()
{
    delete d;
}

void gnomonMeshVtkVisualizationCommand::newVisualization(void)
{
    this->clear();
    auto visu = gnomonVisualization::meshVtkVisualization::pluginFactory().create(this->visu_name);
    this->visu = std::shared_ptr<gnomonAbstractMeshVtkVisualization>(visu);
    this->connectVisualization();
}

void gnomonMeshVtkVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setVisualizationName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractMeshVtkVisualization>(this->visu);
    if (visu) {
        visu->setMesh(d->mesh);
        this->setVisualizationParameters(parameters);
        visu->refreshParameters();
        visu->update();
    }
}

void gnomonMeshVtkVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->mesh = std::dynamic_pointer_cast<gnomonMeshSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonMeshVtkVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["mesh"] = d->mesh;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonMeshVtkVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("mesh", "gnomonMesh"));
    return input_types;
}

void gnomonMeshVtkVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "mesh") {
        auto mesh = std::dynamic_pointer_cast<gnomonMeshSeries>(form);
        d->mesh = mesh;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonMeshVtkVisualizationCommand.cpp ends here
