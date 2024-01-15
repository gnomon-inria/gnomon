#include "gnomonFormAlgorithmCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractFormAlgorithm>
#include <gnomonCore/gnomonForm/gnomonDynamicFormFactory>

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
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;

    /*
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
*/
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
    d->inputs.clear();
    d->outputs.clear();
}

void gnomonFormAlgorithmCommand::addInput(std::shared_ptr<gnomonAbstractDynamicForm> input)
{
    d->inputs[input->formName()] = input;
    ((gnomonAbstractFormAlgorithm *) this->action)->refreshParameters();
}

void gnomonFormAlgorithmCommand::addOutput(std::shared_ptr<gnomonAbstractDynamicForm> output)
{
    d->outputs[output->formName()] = output;
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
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > out_inputs;
    for(auto input : d->inputs) {
        QString form_name = input->formName().replace(QString("gnomon"), QString("input"));
        out_inputs[input->formName()] = input;
    }
    return out_inputs;
}

gnomonAbstractCommand::orderedMap gnomonFormAlgorithmCommand::inputTypes()
{
    orderedMap input_types;
    for(auto input : d->inputs.keys()) {
        QString form_name = input.replace(QString("gnomon"), QString("input"));
        input_types.emplace_back(std::make_pair(form_name, input));
    }
    return input_types;
}

void gnomonFormAlgorithmCommand::setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    this->addInput(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonFormAlgorithmCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > out_outputs;
    for(auto output : d->outputs) {
        QString form_name = output->formName().replace(QString("gnomon"), QString("output"));
        out_outputs[output->formName()] = output;
    }

    return out_outputs;
}

gnomonAbstractCommand::orderedMap gnomonFormAlgorithmCommand::outputTypes()
{
    orderedMap output_types;
    for(auto output : d->outputs.keys()) {
        QString form_name = output.replace(QString("gnomon"), QString("output"));
        output_types.emplace_back(std::make_pair(form_name, output));
    }

    return output_types;
}

void gnomonFormAlgorithmCommand::deserializeResults(QJsonObject &serialization) {
    for (QString form_name : gnomonForm::forms_names) {
        QString form = form_name.replace(QString("gnomon"), QString("output"));
        if (serialization.contains(form)) {
            auto formSerialization = serialization[form].toObject();
            auto formObject = gnomonForm::createDynamicForm(form);
            formObject->deserialize(formSerialization);
        }
    }
}

QJsonObject gnomonFormAlgorithmCommand::serializeResults(void) {
    QJsonObject outputs_json;

    for(auto output : d->outputs) {
        QString output_name = output->formName().replace(QString("gnomon"), QString("output"));
        outputs_json[output_name] = output->serialize();
    }
    return outputs_json;
}
//
// gnomonFormAlgorithmCommand.cpp ends here
