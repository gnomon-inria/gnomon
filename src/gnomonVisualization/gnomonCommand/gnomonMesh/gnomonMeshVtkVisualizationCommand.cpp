#include "gnomonMeshVtkVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonMesh/gnomonAbstractVisualizationMesh.h>
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
    this->factory = &gnomonVisualization::visualizationMesh::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        auto visu = gnomonVisualization::visualizationMesh::pluginFactory().create(this->algorithm_name);
        this->visu = std::shared_ptr<gnomonAbstractVisualizationMesh>(visu);
    }
}

gnomonMeshVtkVisualizationCommand::~gnomonMeshVtkVisualizationCommand()
{
    delete d;
}

void gnomonMeshVtkVisualizationCommand::setAlgorithmName(const QString& visu_name)
{
    if (this->algorithm_name != visu_name) {
        gnomonAbstractVtkVisualizationCommand::setAlgorithmName(visu_name);
        auto visu = gnomonVisualization::visualizationMesh::pluginFactory().create(visu_name);
        this->visu = std::shared_ptr<gnomonAbstractVisualizationMesh>(visu);
    }
}

void gnomonMeshVtkVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setAlgorithmName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractVisualizationMesh>(this->visu);
    if (visu) {
        if (visu->mesh() != d->mesh) {
            visu->setMesh(d->mesh);
            this->setVisualizationParameters(parameters);
        }
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
