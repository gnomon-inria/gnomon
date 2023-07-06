#include "gnomonFormAlgorithmCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormAlgorithm>

#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud>
#include <gnomonCore/gnomonForm/gnomonTree/gnomonTree>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonFormAlgorithmCommandPrivate
{
public:
    std::shared_ptr<gnomonCellComplexSeries> inputCellComplex;
    std::shared_ptr<gnomonCellImageSeries> inputCellImage;
    std::shared_ptr<gnomonImageSeries> inputImage;
    std::shared_ptr<gnomonMeshSeries> inputMesh;
    std::shared_ptr<gnomonPointCloudSeries> inputPointCloud;

    std::shared_ptr<gnomonCellComplexSeries> outputCellComplex;
    std::shared_ptr<gnomonCellImageSeries> outputCellImage;
    std::shared_ptr<gnomonImageSeries> outputImage;
    std::shared_ptr<gnomonMeshSeries> outputMesh;
    std::shared_ptr<gnomonPointCloudSeries> outputPointCloud;

    dtkCoreParameters parameters;

    QString python_code;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonFormAlgorithmCommand::gnomonFormAlgorithmCommand(const QString& key) : d(new gnomonFormAlgorithmCommandPrivate)
{
    this->factory_name = "formAlgorithm";
    this->algorithm_name = key;
}

gnomonFormAlgorithmCommand::~gnomonFormAlgorithmCommand()
{
    delete d;
}

void gnomonFormAlgorithmCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
}

void gnomonFormAlgorithmCommand::setFormAlgorithm(gnomonAbstractFormAlgorithm *algo)
{
    this->action = algo;
}

void gnomonFormAlgorithmCommand::setPythonCode(const QString& code)
{
    d->python_code = code;
}

const QString& gnomonFormAlgorithmCommand::pythonCode(void) const
{
    return d->python_code;
}

gnomonAbstractFormAlgorithm *gnomonFormAlgorithmCommand::formAlgorithm(void)
{
    return dynamic_cast<gnomonAbstractFormAlgorithm *>(this->action);
}

void gnomonFormAlgorithmCommand::predo(void)
{

}

void gnomonFormAlgorithmCommand::postdo(void)
{

}

void gnomonFormAlgorithmCommand::undo(void)
{

}

void gnomonFormAlgorithmCommand::addInput(std::shared_ptr<gnomonAbstractDynamicForm> input)
{
    if (std::shared_ptr<gnomonCellComplexSeries> cellComplex = std::dynamic_pointer_cast<gnomonCellComplexSeries>(input)) {
        d->inputCellComplex = cellComplex;
    }
    if (std::shared_ptr<gnomonCellImageSeries> cellImage = std::dynamic_pointer_cast<gnomonCellImageSeries>(input)) {
        d->inputCellImage = cellImage;
    }
    if (std::shared_ptr<gnomonImageSeries> image = std::dynamic_pointer_cast<gnomonImageSeries>(input)) {
        d->inputImage = image;
    }
    if (std::shared_ptr<gnomonMeshSeries> mesh = std::dynamic_pointer_cast<gnomonMeshSeries>(input)) {
        d->inputMesh = mesh;
    }
    if (std::shared_ptr<gnomonPointCloudSeries> pointCloud = std::dynamic_pointer_cast<gnomonPointCloudSeries>(input)) {
        d->inputPointCloud = pointCloud;
    }
    ((gnomonAbstractFormAlgorithm *) this->action)->refreshParameters();
}

void gnomonFormAlgorithmCommand::addOutput(std::shared_ptr<gnomonAbstractDynamicForm> output)
{
    if (std::shared_ptr<gnomonCellComplexSeries> cellComplex = std::dynamic_pointer_cast<gnomonCellComplexSeries>(output)) {
        d->outputCellComplex = cellComplex;
    }
    if (std::shared_ptr<gnomonCellImageSeries> cellImage = std::dynamic_pointer_cast<gnomonCellImageSeries>(output)) {
        d->outputCellImage = cellImage;
    }
    if (std::shared_ptr<gnomonImageSeries> image = std::dynamic_pointer_cast<gnomonImageSeries>(output)) {
        d->outputImage = image;
    }
    if (std::shared_ptr<gnomonMeshSeries> mesh = std::dynamic_pointer_cast<gnomonMeshSeries>(output)) {
        d->outputMesh = mesh;
    }
    if (std::shared_ptr<gnomonPointCloudSeries> pointCloud = std::dynamic_pointer_cast<gnomonPointCloudSeries>(output)) {
        d->outputPointCloud = pointCloud;
    }
}

void gnomonFormAlgorithmCommand::addParameter(const QString& name, dtkCoreParameter *parameter)
{
    d->parameters[name] = parameter;
}

dtkCoreParameters gnomonFormAlgorithmCommand::parameters() const
{
    return d->parameters;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonFormAlgorithmCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    if (d->inputCellComplex) {
        inputs["inputCellComplex"] = d->inputCellComplex;
    }
    if (d->inputCellImage) {
        inputs["inputCellImage"] = d->inputCellImage;
    }
    if (d->inputImage) {
        inputs["inputImage"] = d->inputImage;
    }
    if (d->inputMesh) {
        inputs["inputMesh"] = d->inputMesh;
    }
    if (d->inputPointCloud) {
        inputs["inputPointCloud"] = d->inputPointCloud;
    }
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonFormAlgorithmCommand::inputTypes()
{
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("inputCellComplex", "gnomonCellComplex"));
    input_types.emplace_back(std::make_pair("inputCellImage", "gnomonCellImage"));
    input_types.emplace_back(std::make_pair("inputImage", "gnomonImage"));
    input_types.emplace_back(std::make_pair("inputMesh", "gnomonMesh"));
    input_types.emplace_back(std::make_pair("inputPointCloud", "gnomonPontCloud"));
    return input_types;
}

void gnomonFormAlgorithmCommand::setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    this->addInput(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonFormAlgorithmCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    if (d->outputCellComplex) {
        outputs["outputCellComplex"] = d->outputCellComplex;
    }
    if (d->outputCellImage) {
        outputs["outputCellImage"] = d->outputCellImage;
    }
    if (d->outputImage) {
        outputs["outputImage"] = d->outputImage;
    }
    if (d->outputMesh) {
        outputs["outputMesh"] = d->outputMesh;
    }
    if (d->outputPointCloud) {
        outputs["outputPointCloud"] = d->outputPointCloud;
    }
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonFormAlgorithmCommand::outputTypes()
{
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("outputCellComplex", "gnomonCellComplex"));
    output_types.emplace_back(std::make_pair("outputCellImage", "gnomonCellImage"));
    output_types.emplace_back(std::make_pair("outputImage", "gnomonImage"));
    output_types.emplace_back(std::make_pair("outputMesh", "gnomonMesh"));
    output_types.emplace_back(std::make_pair("outputPointCloud", "gnomonPontCloud"));
    return output_types;
}

void gnomonFormAlgorithmCommand::deserializeResults(QJsonObject &serialization) {
    if (serialization.contains("outputCellComplex")) {
        d->outputCellComplex = std::make_shared<gnomonCellComplexSeries>();
        auto cellComplexSerialization = serialization["outputCellComplex"].toObject();
        d->outputCellComplex->deserialize(cellComplexSerialization);
    }
    if (serialization.contains("outputCellImage")) {
        d->outputCellImage = std::make_shared<gnomonCellImageSeries>();
        auto cellImageSerialization = serialization["outputCellImage"].toObject();
        d->outputCellImage->deserialize(cellImageSerialization);
    }
    if (serialization.contains("outputImage")) {
        d->outputImage = std::make_shared<gnomonImageSeries>();
        auto imageSerialization = serialization["outputImage"].toObject();
        d->outputImage->deserialize(imageSerialization);
    }
    if (serialization.contains("outputMesh")) {
        d->outputMesh = std::make_shared<gnomonMeshSeries>();
        auto meshSerialization = serialization["outputMesh"].toObject();
        d->outputMesh->deserialize(meshSerialization);
    }
    if (serialization.contains("outputPointCloud")) {
        d->outputPointCloud = std::make_shared<gnomonPointCloudSeries>();
        auto pointCloudSerialization = serialization["outputPointCloud"].toObject();
        d->outputPointCloud->deserialize(pointCloudSerialization);
    }
}

QJsonObject gnomonFormAlgorithmCommand::serializeResults(void) {
    QJsonObject outputs;
    if (d->outputCellComplex) {
        outputs["outputCellComplex"] = d->outputCellComplex->serialize();
    }
    if (d->outputCellImage) {
        outputs["outputCellImage"] = d->outputCellImage->serialize();
    }
    if (d->outputImage) {
        outputs["outputImage"] = d->outputImage->serialize();
    }
    if (d->outputMesh) {
        outputs["outputMesh"] = d->outputMesh->serialize();
    }
    if (d->outputPointCloud) {
        outputs["outputPointCloud"] = d->outputPointCloud->serialize();
    }
    return outputs;
}
//
// gnomonFormAlgorithmCommand.cpp ends here
