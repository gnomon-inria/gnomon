#include "gnomonLStringVtkVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonLString/gnomonAbstractLStringVtkVisualization.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonLStringVtkVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonLStringSeries> lString = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonLStringVtkVisualizationCommand::gnomonLStringVtkVisualizationCommand() : d(new gnomonLStringVtkVisualizationCommandPrivate)
{
    this->factory_name = groupName;
    this->factory = &gnomonVisualization::lStringVtkVisualization::pluginFactory();
    loadPluginGroup(this->factoryName());

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->visu_name = keys[0];
    }
}

gnomonLStringVtkVisualizationCommand::~gnomonLStringVtkVisualizationCommand()
{
    delete d;
}

void gnomonLStringVtkVisualizationCommand::newVisualization(void)
{
    this->clear();
    auto visu = gnomonVisualization::lStringVtkVisualization::pluginFactory().create(this->visu_name);
    this->visu = std::shared_ptr<gnomonAbstractLStringVtkVisualization>(visu);
    this->connectVisualization();
}

void gnomonLStringVtkVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setVisualizationName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractLStringVtkVisualization>(this->visu);
    if (visu) {
        visu->setLString(d->lString);
        this->setVisualizationParameters(parameters);
        visu->refreshParameters();
        visu->update();
    }
}

void gnomonLStringVtkVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->lString = std::dynamic_pointer_cast<gnomonLStringSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringVtkVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["lString"] = d->lString;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonLStringVtkVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("lString", "gnomonLString"));
    return input_types;
}

void gnomonLStringVtkVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "lString") {
        auto lString = std::dynamic_pointer_cast<gnomonLStringSeries>(form);
        d->lString = lString;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonLStringVtkVisualizationCommand.cpp ends here
