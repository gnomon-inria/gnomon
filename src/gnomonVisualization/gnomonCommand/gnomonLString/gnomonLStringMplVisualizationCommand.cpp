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
    this->factory = &gnomonVisualization::lStringMplVisualization::pluginFactory();
    GNOMON_COMMANDS_INIT(gnomonLStringMplVisualizationCommand)

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->visu_name = keys[0];
    }
}

gnomonLStringMplVisualizationCommand::~gnomonLStringMplVisualizationCommand()
{
    delete d;
}

void gnomonLStringMplVisualizationCommand::newVisualization(void)
{
    gnomonAbstractVisualizationCommand::clear();
    auto visu = gnomonVisualization::lStringMplVisualization::pluginFactory().create(this->visu_name);
    this->visu = std::shared_ptr<gnomonAbstractLStringMplVisualization>(visu);
    this->connectVisualization();
}

void gnomonLStringMplVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setVisualizationName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractLStringMplVisualization>(this->visu);
    if (visu) {
        if (d->lString) {
            d->lString->load();
        }
        visu->setLString(d->lString);
        this->setVisualizationParameters(parameters);
        visu->refreshParameters();
        visu->update();
    }
}

void gnomonLStringMplVisualizationCommand::predo(void)
{
    std::dynamic_pointer_cast<gnomonAbstractLStringMplVisualization>(this->visu)->setLString(d->lString);
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

void gnomonLStringMplVisualizationCommand::clear(void) {
    gnomonAbstractVisualizationCommand::clear();
    d->lString = nullptr;
}

//
// gnomonLStringMplVisualizationCommand.cpp ends here
