// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonFormAlgorithmCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonFormAlgorithmCommandPrivate
{
public:
    gnomonCellComplexSeries* inputCellComplex = nullptr;
    gnomonCellImageSeries* inputCellImage = nullptr;
    gnomonImageSeries* inputImage = nullptr;
    gnomonMeshSeries* inputMesh = nullptr;
    gnomonPointCloudSeries* inputPointCloud = nullptr;

    gnomonCellComplexSeries* outputCellComplex = nullptr;
    gnomonCellImageSeries* outputCellImage = nullptr;
    gnomonImageSeries* outputImage = nullptr;
    gnomonMeshSeries* outputMesh = nullptr;
    gnomonPointCloudSeries* outputPointCloud = nullptr;

    dtkCoreParameters parameters;
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

void gnomonFormAlgorithmCommand::predo(void)
{

}

void gnomonFormAlgorithmCommand::postdo(void)
{

}

void gnomonFormAlgorithmCommand::undo(void)
{

}

void gnomonFormAlgorithmCommand::addInput(gnomonAbstractDynamicForm *input)
{
    if (gnomonCellComplexSeries* cellComplex = dynamic_cast<gnomonCellComplexSeries *>(input)) {
        d->inputCellComplex = cellComplex;
    }
    if (gnomonCellImageSeries* cellImage = dynamic_cast<gnomonCellImageSeries *>(input)) {
        d->inputCellImage = cellImage;
    }
    if (gnomonImageSeries* image = dynamic_cast<gnomonImageSeries *>(input)) {
        d->inputImage = image;
    }
    if (gnomonMeshSeries* mesh = dynamic_cast<gnomonMeshSeries *>(input)) {
        d->inputMesh = mesh;
    }
    if (gnomonPointCloudSeries* pointCloud = dynamic_cast<gnomonPointCloudSeries *>(input)) {
        d->inputPointCloud = pointCloud;
    }
}

void gnomonFormAlgorithmCommand::addOutput(gnomonAbstractDynamicForm *output)
{
    if (gnomonCellComplexSeries* cellComplex = dynamic_cast<gnomonCellComplexSeries *>(output)) {
        d->outputCellComplex = cellComplex;
    }
    if (gnomonCellImageSeries* cellImage = dynamic_cast<gnomonCellImageSeries *>(output)) {
        d->outputCellImage = cellImage;
    }
    if (gnomonImageSeries* image = dynamic_cast<gnomonImageSeries *>(output)) {
        d->outputImage = image;
    }
    if (gnomonMeshSeries* mesh = dynamic_cast<gnomonMeshSeries *>(output)) {
        d->outputMesh = mesh;
    }
    if (gnomonPointCloudSeries* pointCloud = dynamic_cast<gnomonPointCloudSeries *>(output)) {
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

QMap<QString, gnomonAbstractDynamicForm *> gnomonFormAlgorithmCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
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

void gnomonFormAlgorithmCommand::setInputForm(const QString& name, gnomonAbstractDynamicForm *form)
{
    this->addInput(form);
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonFormAlgorithmCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
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


//
// gnomonFormAlgorithmCommand.cpp ends here
