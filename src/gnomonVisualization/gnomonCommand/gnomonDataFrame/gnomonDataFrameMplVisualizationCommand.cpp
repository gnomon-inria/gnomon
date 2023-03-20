#include "gnomonDataFrameMplVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonDataFrame/gnomonAbstractDataFrameMplVisualization.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonDataFrameMplVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonDataFrameSeries> dataFrame = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonDataFrameMplVisualizationCommand::gnomonDataFrameMplVisualizationCommand() : d(new gnomonDataFrameMplVisualizationCommandPrivate)
{
    this->factory_name = groupName;
    this->factory = &gnomonVisualization::dataFrameMplVisualization::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        auto visu = gnomonVisualization::dataFrameMplVisualization::pluginFactory().create(this->algorithm_name);
        this->visu = std::shared_ptr<gnomonAbstractDataFrameMplVisualization>(visu);
    }
}

gnomonDataFrameMplVisualizationCommand::~gnomonDataFrameMplVisualizationCommand()
{
    delete d;
}

void gnomonDataFrameMplVisualizationCommand::setAlgorithmName(const QString& visu_name)
{
    this->algorithm_name = visu_name;
    this->visu->deleteLater();
    auto visu = gnomonVisualization::dataFrameMplVisualization::pluginFactory().create(visu_name);
    this->visu = std::shared_ptr<gnomonAbstractDataFrameMplVisualization>(visu);
}

void gnomonDataFrameMplVisualizationCommand::predo(void)
{
    std::dynamic_pointer_cast<gnomonAbstractDataFrameMplVisualization>(this->visu)->setDataFrame(d->dataFrame->current());
}

void gnomonDataFrameMplVisualizationCommand::postdo(void)
{

}

void gnomonDataFrameMplVisualizationCommand::undo()
{
}

void gnomonDataFrameMplVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->dataFrame = std::dynamic_pointer_cast<gnomonDataFrameSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonDataFrameMplVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["dataFrame"] = d->dataFrame;
    return inputs;
}

bool gnomonDataFrameMplVisualizationCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonDataFrameMplVisualizationCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonDataFrameMplVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("dataFrame", "gnomonDataFrame"));
    return input_types;
}

void gnomonDataFrameMplVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "dataFrame") {
        auto dataFrame = std::dynamic_pointer_cast<gnomonDataFrameSeries>(form);
        d->dataFrame = dataFrame;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonDataFrameMplVisualizationCommand.cpp ends here
