#include "gnomonCellImageFromImageCommand.h"

#include <dtkLog>
#include <dtkScript>
#include <dtkImagingCore>

class gnomonCellImageFromImageCommandPrivate
{
public:
    gnomonImageSeries *image_series = nullptr;
    gnomonPointCloudSeries *pointCloud_series = nullptr;

    gnomonCellImageSeries *output = nullptr;
};

gnomonCellImageFromImageCommand::gnomonCellImageFromImageCommand(void) : d(new gnomonCellImageFromImageCommandPrivate)
{
    this->factory_name = "cellImageFromImage";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::cellImageFromImage::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellImageFromImage::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellImageFromImageCommand::~gnomonCellImageFromImageCommand(void)
{
    delete d;
}

void gnomonCellImageFromImageCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::cellImageFromImage::pluginFactory().create(algo_name);
}

void gnomonCellImageFromImageCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonCellImageSeries *cellImage = ((gnomonAbstractCellImageFromImage *) this->action)->output();
    if ((!cellImage)||(cellImage->times().size())==0) {
        d->output = nullptr;
    } else {
        d->output = cellImage;
    }
}

void gnomonCellImageFromImageCommand::undo(void)
{
    Q_ASSERT(this->action);

    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(nullptr);
    ((gnomonAbstractCellImageFromImage *) this->action)->setCellPoints(nullptr);
}

void gnomonCellImageFromImageCommand::setInput(gnomonImageSeries* image_series)
{
    if ((!image_series)||(image_series->times().size()==0)||(((gnomonImage *)image_series->current())->channels().size()==0)) {
        d->image_series = nullptr;
    } else {
        d->image_series = image_series;
    }
    ((gnomonAbstractCellImageFromImage *) this->action)->setInput(d->image_series);
}

gnomonImageSeries *gnomonCellImageFromImageCommand::input()
{
    return d->image_series;
}

void gnomonCellImageFromImageCommand::setCellPoints(gnomonPointCloudSeries *pointCloud_series)
{
    if ((!pointCloud_series)||(pointCloud_series->times().size()==0)) {
        d->pointCloud_series = nullptr;
    } else {
        d->pointCloud_series = pointCloud_series;
    }
    ((gnomonAbstractCellImageFromImage *) this->action)->setCellPoints(d->pointCloud_series);
}

gnomonPointCloudSeries *gnomonCellImageFromImageCommand::cellPoints()
{
    return d->pointCloud_series;
}

gnomonCellImageSeries *gnomonCellImageFromImageCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageFromImageCommand::inputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    inputs["cellPoints"] = this->cellPoints();
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonCellImageFromImageCommand::inputTypes(void)
{
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("input", "gnomonImage"));
    input_types.emplace_back(std::make_pair("cellPoints", "gnomonPointCloud"));
    return input_types;
}

void gnomonCellImageFromImageCommand::setInputForm(const QString& name, gnomonAbstractDynamicForm *form)
{
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonImageSeries *>(form));
    } else if (name == "cellPoints") {
        this->setCellPoints(dynamic_cast<gnomonPointCloudSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonCellImageFromImageCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonCellImageFromImageCommand::outputTypes(void)
{
    orderedMap output_types;
    output_types.emplace_back(std::make_pair("output", "gnomonCellImage"));
    return output_types;
}

dtkCoreParameters gnomonCellImageFromImageCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonCellImageFromImageCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

bool gnomonCellImageFromImageCommand::isEmpty(void)
{
    loadPluginGroup("cellImageFromImage");
    return gnomonCore::cellImageFromImage::pluginFactory().keys().size() == 0;
}

//
// gnomonCellImageFromImageCommand.cpp ends here
