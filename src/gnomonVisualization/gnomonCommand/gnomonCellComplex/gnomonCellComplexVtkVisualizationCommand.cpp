#include "gnomonCellComplexVtkVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonCellComplex/gnomonAbstractCellComplexVtkVisualization.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonCellComplexVtkVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonCellComplexSeries> cellComplex = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellComplexVtkVisualizationCommand::gnomonCellComplexVtkVisualizationCommand() : d(new gnomonCellComplexVtkVisualizationCommandPrivate)
{
    this->factory_name = groupName;
    this->factory = &gnomonVisualization::cellComplexVtkVisualization::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        auto visu = gnomonVisualization::cellComplexVtkVisualization::pluginFactory().create(this->algorithm_name);
        this->visu = std::shared_ptr<gnomonAbstractCellComplexVtkVisualization>(visu);
    }
}

gnomonCellComplexVtkVisualizationCommand::~gnomonCellComplexVtkVisualizationCommand()
{
    delete d;
}

void gnomonCellComplexVtkVisualizationCommand::setAlgorithmName(const QString& visu_name)
{
    if (this->algorithm_name != visu_name) {
        gnomonAbstractVtkVisualizationCommand::setAlgorithmName(visu_name);
        auto visu = gnomonVisualization::cellComplexVtkVisualization::pluginFactory().create(visu_name);
        this->visu = std::shared_ptr<gnomonAbstractCellComplexVtkVisualization>(visu);
    }
}

void gnomonCellComplexVtkVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setAlgorithmName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractCellComplexVtkVisualization>(this->visu);
    if (visu) {
        if (visu->cellComplex() != d->cellComplex) {
            visu->setCellComplex(d->cellComplex);
            this->setVisualizationParameters(parameters);
        }
    }
}

void gnomonCellComplexVtkVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->cellComplex = std::dynamic_pointer_cast<gnomonCellComplexSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellComplexVtkVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["cellComplex"] = d->cellComplex;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonCellComplexVtkVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("cellComplex", "gnomonCellComplex"));
    return input_types;
}

void gnomonCellComplexVtkVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "cellComplex") {
        auto cellComplex = std::dynamic_pointer_cast<gnomonCellComplexSeries>(form);
        d->cellComplex = cellComplex;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonCellComplexVtkVisualizationCommand.cpp ends here
