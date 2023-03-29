#include "gnomonLStringTranslationCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonLString/gnomonAbstractLStringTranslation.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonLStringTranslationCommandPrivate
{
public:
    std::shared_ptr<gnomonTreeSeries> tree_series = nullptr;
    std::shared_ptr<gnomonLStringSeries> lString_series = nullptr;

    std::shared_ptr<gnomonTreeSeries> output_tree_series = nullptr;
    std::shared_ptr<gnomonLStringSeries> output_lString_series = nullptr;
};

gnomonLStringTranslationCommand::gnomonLStringTranslationCommand() : d(new gnomonLStringTranslationCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::lStringTranslation::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::lStringTranslation::pluginFactory().create(this->algorithm_name);
    }

}

gnomonLStringTranslationCommand::~gnomonLStringTranslationCommand()
{
    delete d;
}

void gnomonLStringTranslationCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::lStringTranslation::pluginFactory().create(algo_name);
}

void gnomonLStringTranslationCommand::predo(void)
{

}

void gnomonLStringTranslationCommand::postdo(void)
{
    std::shared_ptr<gnomonTreeSeries> tree = ((gnomonAbstractLStringTranslation *) this->action)->outputTree();

    if ((!tree)||(tree->times().empty())) {
        d->output_tree_series = nullptr;
    } else {
        d->output_tree_series = tree;
    }

    std::shared_ptr<gnomonLStringSeries> lString = ((gnomonAbstractLStringTranslation *) this->action)->outputLString();

    if ((!lString)||(lString->times().empty())) {
        d->output_lString_series = nullptr;
    } else {
        d->output_lString_series = lString;
    }
}

void gnomonLStringTranslationCommand::undo()
{
    Q_ASSERT(this->action);

    ((gnomonAbstractLStringTranslation *) this->action)->setInputTree(nullptr);
    ((gnomonAbstractLStringTranslation *) this->action)->setInputLString(nullptr);
    this->action->refreshParameters();
}

void gnomonLStringTranslationCommand::setInputTree(std::shared_ptr<gnomonTreeSeries> tree_series)
{
    if ((!tree_series)||(tree_series->times().empty())) {
        d->tree_series = nullptr;
    } else {
        d->tree_series = tree_series;
        Q_ASSERT(this->action);
        ((gnomonAbstractLStringTranslation *) this->action)->setInputTree(d->tree_series);
        this->action->refreshParameters();
    }
}

void gnomonLStringTranslationCommand::setInputLString(std::shared_ptr<gnomonLStringSeries> lString_series)
{
    if ((!lString_series)||(lString_series->times().empty())) {
        d->lString_series = nullptr;
    } else {
        d->lString_series = lString_series;
    }
    Q_ASSERT(this->action);
    ((gnomonAbstractLStringTranslation *) this->action)->setInputLString(d->lString_series);
    this->action->refreshParameters();
}

std::shared_ptr<gnomonTreeSeries> gnomonLStringTranslationCommand::inputTree()
{
    return d->tree_series;
}

std::shared_ptr<gnomonLStringSeries> gnomonLStringTranslationCommand::inputLString()
{
    return d->lString_series;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringTranslationCommand::inputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
    inputs["inputTree"] = this->inputTree();
    inputs["inputLString"] = this->inputLString();
    return inputs;
}

std::shared_ptr<gnomonTreeSeries> gnomonLStringTranslationCommand::outputTree()
{
    return d->output_tree_series;
}

std::shared_ptr<gnomonLStringSeries> gnomonLStringTranslationCommand::outputLString()
{
    return d->output_lString_series;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringTranslationCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["outputTree"] = this->outputTree();
    outputs["outputLString"] = this->outputLString();
    return outputs;
}


bool gnomonLStringTranslationCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonLStringTranslationCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonLStringTranslationCommand::inputTypes() {
    orderedMap input_types;
    input_types.emplace_back(std::make_pair("inputTree", "gnomonTree"));
    input_types.emplace_back(std::make_pair("inputLString", "gnomonLString"));
    return input_types;
}

gnomonAbstractCommand::orderedMap gnomonLStringTranslationCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("outputTree", "gnomonTree"));
    types.emplace_back(std::make_pair("outputLString", "gnomonLString"));
    return types;
}

void gnomonLStringTranslationCommand::setInputForm(const QString &name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "inputTree") {
        this->setInputTree(std::dynamic_pointer_cast<gnomonTreeSeries>(form));
    } else if (name == "inputLString") {
        this->setInputLString(std::dynamic_pointer_cast<gnomonLStringSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

void gnomonLStringTranslationCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output_lString_series) {
        d->output_lString_series = std::make_shared<gnomonLStringSeries>();
    }
    auto tmp = serialization["outputLString"].toObject();
    d->output_lString_series->deserialize(tmp);
    if(!d->output_tree_series) {
        d->output_tree_series = std::make_shared<gnomonTreeSeries>();
    }
    auto tmp2 = serialization["outputTree"].toObject();
    d->output_tree_series->deserialize(tmp2);
}

QJsonObject gnomonLStringTranslationCommand::serializeResults(void) {
    QJsonObject out;
    out["outputLString"] = d->output_lString_series->serialize();
    out["outputTree"] = d->output_tree_series->serialize();
    return out;
}

//
// gnomonLStringTranslationCommand.cpp ends here
