#include "gnomonDataDictQmlVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonDataDict/gnomonAbstractDataDictQmlVisualization.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonDataDictQmlVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonDataDictSeries> dataDict = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonDataDictQmlVisualizationCommand::gnomonDataDictQmlVisualizationCommand() : d(new gnomonDataDictQmlVisualizationCommandPrivate)
{
    this->factory_name = groupName;
    this->factory = &gnomonVisualization::dataDictQmlVisualization::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
    }
}

gnomonDataDictQmlVisualizationCommand::~gnomonDataDictQmlVisualizationCommand()
{
    delete d;
}

void gnomonDataDictQmlVisualizationCommand::newVisualization(void)
{
    this->clear();
    auto visu = gnomonVisualization::dataDictQmlVisualization::pluginFactory().create(this->algorithm_name);
    this->visu = std::shared_ptr<gnomonAbstractDataDictQmlVisualization>(visu);
    this->connectVisualization();
}

void gnomonDataDictQmlVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setAlgorithmName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractDataDictQmlVisualization>(this->visu);
    if (visu) {
        visu->setDataDict(d->dataDict);
        this->setVisualizationParameters(parameters);
        visu->refreshParameters();
        visu->update();
    }
}

void gnomonDataDictQmlVisualizationCommand::predo(void)
{
    std::dynamic_pointer_cast<gnomonAbstractDataDictQmlVisualization>(this->visu)->setDataDict(d->dataDict);
}

void gnomonDataDictQmlVisualizationCommand::postdo(void)
{

}

void gnomonDataDictQmlVisualizationCommand::undo()
{
}

void gnomonDataDictQmlVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->dataDict = std::dynamic_pointer_cast<gnomonDataDictSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonDataDictQmlVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["dataDict"] = d->dataDict;
    return inputs;
}

bool gnomonDataDictQmlVisualizationCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonDataDictQmlVisualizationCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonDataDictQmlVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("dataDict", "gnomonDataDict"));
    return input_types;
}

void gnomonDataDictQmlVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "dataDict") {
        auto dataDict = std::dynamic_pointer_cast<gnomonDataDictSeries>(form);
        d->dataDict = dataDict;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonDataDictQmlVisualizationCommand.cpp ends here
