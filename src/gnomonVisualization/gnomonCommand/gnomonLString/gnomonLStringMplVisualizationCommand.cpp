#include "gnomonLStringMplVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonLString/gnomonAbstractLStringMplVisualization.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonLStringMplVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonLStringSeries> lString = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonLStringMplVisualizationCommand::gnomonLStringMplVisualizationCommand() : d(new gnomonLStringMplVisualizationCommandPrivate)
{
    this->factory_name = groupName;
    this->factory = &gnomonVisualization::lStringMplVisualization::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
    }
}

gnomonLStringMplVisualizationCommand::~gnomonLStringMplVisualizationCommand()
{
    delete d;
}

void gnomonLStringMplVisualizationCommand::setAlgorithmName(const QString& visu_name)
{
    if ((this->algorithm_name != visu_name) || (this->visu == nullptr)) {
        gnomonAbstractVisualizationCommand::setAlgorithmName(visu_name);
        auto visu = gnomonVisualization::lStringMplVisualization::pluginFactory().create(visu_name);
        this->visu = std::shared_ptr<gnomonAbstractLStringMplVisualization>(visu);
        this->connectVisualization();
    }
}

void gnomonLStringMplVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setAlgorithmName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractLStringMplVisualization>(this->visu);
    if (visu) {
        if (visu->lString() != d->lString->current()) {
            visu->setLString(d->lString->current());
            this->setVisualizationParameters(parameters);
            visu->update();
        }
    }
}

void gnomonLStringMplVisualizationCommand::predo(void)
{
    std::dynamic_pointer_cast<gnomonAbstractLStringMplVisualization>(this->visu)->setLString(d->lString->current());
}

void gnomonLStringMplVisualizationCommand::postdo(void)
{

}

void gnomonLStringMplVisualizationCommand::undo()
{
}

void gnomonLStringMplVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->lString = std::dynamic_pointer_cast<gnomonLStringSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringMplVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["lString"] = d->lString;
    return inputs;
}

bool gnomonLStringMplVisualizationCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonLStringMplVisualizationCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonLStringMplVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("lString", "gnomonLString"));
    return input_types;
}

void gnomonLStringMplVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "lString") {
        auto lString = std::dynamic_pointer_cast<gnomonLStringSeries>(form);
        d->lString = lString;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonLStringMplVisualizationCommand.cpp ends here
