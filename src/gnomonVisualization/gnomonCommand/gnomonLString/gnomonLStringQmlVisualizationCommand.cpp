#include "gnomonLStringQmlVisualizationCommand.h"

#include <gnomonVisualization/gnomonVisualizations/gnomonLString/gnomonAbstractLStringQmlVisualization.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonLStringQmlVisualizationCommandPrivate
{
public:
    std::shared_ptr<gnomonLStringSeries> lString = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonLStringQmlVisualizationCommand::gnomonLStringQmlVisualizationCommand() : d(new gnomonLStringQmlVisualizationCommandPrivate)
{
    this->factory = &gnomonVisualization::lStringQmlVisualization::pluginFactory();
    GNOMON_COMMANDS_INIT(gnomonLStringQmlVisualizationCommand)

    QStringList keys = this->factory->keys();
    if (!keys.empty()) {
        this->visu_name = keys[0];
    }
}

gnomonLStringQmlVisualizationCommand::~gnomonLStringQmlVisualizationCommand()
{
    delete d;
}

void gnomonLStringQmlVisualizationCommand::newVisualization(void)
{
    gnomonAbstractVisualizationCommand::clear();
    auto visu = gnomonVisualization::lStringQmlVisualization::pluginFactory().create(this->visu_name);
    this->visu = std::shared_ptr<gnomonAbstractLStringQmlVisualization>(visu);
    this->connectVisualization();
}

void gnomonLStringQmlVisualizationCommand::setFormVisualization(const QString& visu_name, const QVariantMap &parameters)
{
    this->setVisualizationName(visu_name);
    auto &&visu = std::static_pointer_cast<gnomonAbstractLStringQmlVisualization>(this->visu);
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

void gnomonLStringQmlVisualizationCommand::predo(void)
{
    std::dynamic_pointer_cast<gnomonAbstractLStringQmlVisualization>(this->visu)->setLString(d->lString);
}

void gnomonLStringQmlVisualizationCommand::postdo(void)
{

}

void gnomonLStringQmlVisualizationCommand::undo()
{
}

void gnomonLStringQmlVisualizationCommand::setForm(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    d->lString = std::dynamic_pointer_cast<gnomonLStringSeries>(form);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringQmlVisualizationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["lString"] = d->lString;
    return inputs;
}

gnomonAbstractCommand::orderedMap gnomonLStringQmlVisualizationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("lString", "gnomonLString"));
    return input_types;
}

void gnomonLStringQmlVisualizationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "lString") {
        auto lString = std::dynamic_pointer_cast<gnomonLStringSeries>(form);
        d->lString = lString;
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonLStringQmlVisualizationCommand::clear(void) {
    gnomonAbstractVisualizationCommand::clear();
    d->lString = nullptr;
}

//
// gnomonLStringQmlVisualizationCommand.cpp ends here
