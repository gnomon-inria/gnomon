#include "gnomonTreeMplVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonTree/gnomonAbstractTreeMplVisualization.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonTreeMplVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonTreeSeries> tree = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeMplVisualizationCommand::gnomonTreeMplVisualizationCommand() : d(new gnomonTreeMplVisualizationCommandPrivate)
{
    this->factory = &gnomonVisualization::treeMplVisualization::pluginFactory();
    GNOMON_COMMANDS_INIT(gnomonTreeMplVisualizationCommand)

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->visu_name = keys[0];
    }
}

gnomonTreeMplVisualizationCommand::~gnomonTreeMplVisualizationCommand()
{
    delete d;
}

void gnomonTreeMplVisualizationCommand::newVisualization(void)
{
    gnomonAbstractVisualizationCommand::clear();
    auto visu = gnomonVisualization::treeMplVisualization::pluginFactory().create(this->visu_name);
    this->visu = std::shared_ptr<gnomonAbstractTreeMplVisualization>(visu);
    this->connectVisualization();
}

void gnomonTreeMplVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setVisualizationName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractTreeMplVisualization>(this->visu);
    if (visu) {
        visu->setTree(d->tree);
        this->setVisualizationParameters(parameters);
        visu->refreshParameters();
        visu->update();
    }
}

void gnomonTreeMplVisualizationCommand::predo(void)
{
    std::dynamic_pointer_cast<gnomonAbstractTreeMplVisualization>(this->visu)->setTree(d->tree);
}

void gnomonTreeMplVisualizationCommand::postdo(void)
{

}

void gnomonTreeMplVisualizationCommand::undo()
{
}

void gnomonTreeMplVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->tree = std::dynamic_pointer_cast<gnomonTreeSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonTreeMplVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["tree"] = d->tree;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonTreeMplVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("tree", "gnomonTree"));
    return input_types;
}

void gnomonTreeMplVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "tree") {
        auto tree = std::dynamic_pointer_cast<gnomonTreeSeries>(form);
        d->tree = tree;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonTreeMplVisualizationCommand::clear(void) {
    gnomonAbstractVisualizationCommand::clear();
    d->tree = nullptr;
}

//
// gnomonTreeMplVisualizationCommand.cpp ends here
