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
    this->factory = &gnomonVisualization::dataFrameMplVisualization::pluginFactory();
    GNOMON_COMMANDS_INIT(gnomonDataFrameMplVisualizationCommand)

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->visu_name = keys[0];
    }
}

gnomonDataFrameMplVisualizationCommand::~gnomonDataFrameMplVisualizationCommand()
{
    delete d;
}

void gnomonDataFrameMplVisualizationCommand::newVisualization(void)
{
    gnomonAbstractVisualizationCommand::clear();
    auto visu = gnomonVisualization::dataFrameMplVisualization::pluginFactory().create(this->visu_name);
    this->visu = std::shared_ptr<gnomonAbstractDataFrameMplVisualization>(visu);
    this->connectVisualization();
}

void gnomonDataFrameMplVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setVisualizationName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractDataFrameMplVisualization>(this->visu);
    if (visu) {
        if (d->dataFrame) {
            d->dataFrame->load();
        }
        visu->setDataFrame(d->dataFrame);
        this->setVisualizationParameters(parameters);
        visu->refreshParameters();
        visu->update();
    }
}

void gnomonDataFrameMplVisualizationCommand::predo(void)
{
    std::dynamic_pointer_cast<gnomonAbstractDataFrameMplVisualization>(this->visu)->setDataFrame(d->dataFrame);
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

void gnomonDataFrameMplVisualizationCommand::clear(void) {
    gnomonAbstractVisualizationCommand::clear();
    d->dataFrame = nullptr;
}

void gnomonDataFrameMplVisualizationCommand::clear(bool clear_visu) {
    gnomonAbstractVisualizationCommand::clear(clear_visu);
    d->dataFrame = nullptr;
}

//
// gnomonDataFrameMplVisualizationCommand.cpp ends here
