#include "gnomonPointCloudQuantificationCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonPointCloud/gnomonAbstractPointCloudQuantification.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonPointCloudQuantificationCommandPrivate
{
public:
    std::shared_ptr<gnomonImageSeries> image;
    std::shared_ptr<gnomonMeshSeries> mesh;
    std::shared_ptr<gnomonPointCloudSeries> input_pointCloud;

    std::shared_ptr<gnomonPointCloudSeries> pointCloud;
    std::shared_ptr<gnomonDataFrameSeries> dataFrame;
};

gnomonPointCloudQuantificationCommand::gnomonPointCloudQuantificationCommand() : d(new gnomonPointCloudQuantificationCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonPointCloudQuantificationCommand)

    QStringList keys = gnomonCore::pointCloudQuantification::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::pointCloudQuantification::pluginFactory().create(this->algorithm_name);
    }
}

gnomonPointCloudQuantificationCommand::~gnomonPointCloudQuantificationCommand()
{
    delete d;
}

void gnomonPointCloudQuantificationCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::pointCloudQuantification::pluginFactory().create(algo_name);
}

void gnomonPointCloudQuantificationCommand::predo(void)
{

}

void gnomonPointCloudQuantificationCommand::postdo(void)
{
    std::shared_ptr<gnomonPointCloudSeries> pointCloud = ((gnomonAbstractPointCloudQuantification *) this->action)->pointCloud();

    if ((!pointCloud)||pointCloud->times().empty()) {
        d->pointCloud = nullptr;
    } else {
        d->pointCloud = pointCloud;
    }

    std::shared_ptr<gnomonDataFrameSeries> dataFrame = ((gnomonAbstractPointCloudQuantification *) this->action)->dataFrame();

    if ((!dataFrame)||(dataFrame->times().empty())) {
        d->dataFrame = nullptr;
    }
    else {
        d->dataFrame = dataFrame;
    }
}

void gnomonPointCloudQuantificationCommand::undo()
{
    ((gnomonAbstractPointCloudQuantification *) this->action)->setImage(nullptr);
    ((gnomonAbstractPointCloudQuantification *) this->action)->setMesh(nullptr);
    ((gnomonAbstractPointCloudQuantification *) this->action)->setPointCloud(nullptr);
    this->action->refreshParameters();
}

void gnomonPointCloudQuantificationCommand::setImage(std::shared_ptr<gnomonImageSeries> image)
{
    if ((!image)||(image->times().empty())||(image->current()->channels().empty())) {
        d->image = nullptr;
    } else {
        d->image = image;
    }
    Q_ASSERT(this->action);
    if (d->image) {
        d->image->load();
    }
    ((gnomonAbstractPointCloudQuantification *) this->action)->setImage(d->image);
    this->action->refreshParameters();
}

void gnomonPointCloudQuantificationCommand::setMesh(std::shared_ptr<gnomonMeshSeries> mesh)
{
    if ((!mesh)||(mesh->times().empty())) {
        d->mesh = nullptr;
    } else {
        d->mesh = mesh;
    }
    Q_ASSERT(this->action);
    if (d->mesh) {
        d->mesh->load();
    }
    ((gnomonAbstractPointCloudQuantification *) this->action)->setMesh(d->mesh);
    this->action->refreshParameters();
}

void gnomonPointCloudQuantificationCommand::setPointCloud(std::shared_ptr<gnomonPointCloudSeries> pointCloud)
{
    if ((!pointCloud)||(pointCloud->times().empty())) {
        d->input_pointCloud = nullptr;
    } else {
        d->input_pointCloud = pointCloud;
    }
    Q_ASSERT(this->action);
    if (d->input_pointCloud) {
        d->input_pointCloud->load();
    }
    ((gnomonAbstractPointCloudQuantification *) this->action)->setPointCloud(d->input_pointCloud);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonPointCloudSeries> gnomonPointCloudQuantificationCommand::pointCloud()
{
    return d->pointCloud;
}

std::shared_ptr<gnomonDataFrameSeries> gnomonPointCloudQuantificationCommand::dataFrame()
{
    return d->dataFrame;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonPointCloudQuantificationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["image"] = d->image;
    inputs["mesh"] = d->mesh;
    inputs["pointCloud"] = d->input_pointCloud;
    return inputs;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonPointCloudQuantificationCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["pointCloud"] = this->pointCloud();
    outputs["dataFrame"] = this->dataFrame();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonPointCloudQuantificationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("image", "gnomonImage"));
    input_types.emplace_back(std::make_pair("mesh", "gnomonMesh"));
    input_types.emplace_back(std::make_pair("pointCloud", "gnomonPointCloud"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonPointCloudQuantificationCommand::outputTypes() {
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("pointCloud", "gnomonPointCloud"));
    output_types.emplace_back(std::make_pair("dataFrame", "gnomonDataFrame"));
    return output_types;
}

void gnomonPointCloudQuantificationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "image") {
        this->setImage(std::dynamic_pointer_cast<gnomonImageSeries>(form));
    } else if (name == "mesh") {
        this->setMesh(std::dynamic_pointer_cast<gnomonMeshSeries>(form));
    } else if (name == "pointCloud") {
        this->setPointCloud(std::dynamic_pointer_cast<gnomonPointCloudSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonPointCloudQuantificationCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->pointCloud) {
        d->pointCloud = std::make_shared<gnomonPointCloudSeries>();
    }
    auto tmp = serialization["pointCloud"].toObject();
    d->pointCloud->deserialize(tmp);
    if(!d->dataFrame) {
        d->dataFrame = std::make_shared<gnomonDataFrameSeries>();
    }
    auto tmp2 = serialization["dataFrame"].toObject();
    d->dataFrame->deserialize(tmp2);
}

QJsonObject gnomonPointCloudQuantificationCommand::serializeResults(void) {
    QJsonObject out;
    out["pointCloud"] = d->pointCloud->serialize();
    out["dataFrame"] = d->dataFrame->serialize();
    return out;
}

void gnomonPointCloudQuantificationCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->mesh = nullptr;
    d->dataFrame = nullptr;
    d->pointCloud = nullptr;
    d->image = nullptr;
    d->input_pointCloud = nullptr;
}

//
// gnomonPointCloudQuantificationCommand.cpp ends here
