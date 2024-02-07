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
    this->factory = &gnomonVisualization::dataDictQmlVisualization::pluginFactory();
    GNOMON_COMMANDS_INIT(gnomonDataDictQmlVisualizationCommand)

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->visu_name = keys[0];
    }
}

gnomonDataDictQmlVisualizationCommand::~gnomonDataDictQmlVisualizationCommand()
{
    delete d;
}

void gnomonDataDictQmlVisualizationCommand::newVisualization(void)
{
    gnomonAbstractVisualizationCommand::clear();
    auto visu = gnomonVisualization::dataDictQmlVisualization::pluginFactory().create(this->visu_name);
    this->visu = std::shared_ptr<gnomonAbstractDataDictQmlVisualization>(visu);
    this->connectVisualization();
}

void gnomonDataDictQmlVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setVisualizationName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractDataDictQmlVisualization>(this->visu);
    if (visu) {
        if (d->dataDict) {
            d->dataDict->load();
        }
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

void gnomonDataDictQmlVisualizationCommand::clear(void) {
    gnomonAbstractVisualizationCommand::clear();
    d->dataDict = nullptr;
}

//
// gnomonDataDictQmlVisualizationCommand.cpp ends here
