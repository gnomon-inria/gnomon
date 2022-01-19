// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonLStringTranslationCommand.h"

#include <dtkScript>
#include <dtkImagingCore>

class gnomonLStringTranslationCommandPrivate
{
public:
    gnomonTreeSeries *tree_series = nullptr;
    gnomonLStringSeries *lString_series = nullptr;

    gnomonTreeSeries *output_tree_series = nullptr;
    gnomonLStringSeries *output_lString_series = nullptr;
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

void gnomonLStringTranslationCommand::redo()
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonTreeSeries *tree = ((gnomonAbstractLStringTranslation *) this->action)->outputTree();
    qDebug()<<Q_FUNC_INFO<<tree;
    if ((!tree)||(tree->times().empty())) {
        d->output_tree_series = nullptr;
    } else {
        d->output_tree_series = tree;
    }

    gnomonLStringSeries *lString = ((gnomonAbstractLStringTranslation *) this->action)->outputLString();
    qDebug()<<Q_FUNC_INFO<<lString;
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
}

void gnomonLStringTranslationCommand::setInputTree(gnomonTreeSeries* tree_series)
{
    if ((!tree_series)||(tree_series->times().empty())) {
        d->tree_series = nullptr;
    } else {
        d->tree_series = tree_series;
        Q_ASSERT(this->action);
        ((gnomonAbstractLStringTranslation *) this->action)->setInputTree(d->tree_series);
    }
}

void gnomonLStringTranslationCommand::setInputLString(gnomonLStringSeries* lString_series)
{
    if ((!lString_series)||(lString_series->times().empty())) {
        d->lString_series = nullptr;
    } else {
        d->lString_series = lString_series;
        Q_ASSERT(this->action);
        ((gnomonAbstractLStringTranslation *) this->action)->setInputLString(d->lString_series);
    }
}

gnomonTreeSeries *gnomonLStringTranslationCommand::inputTree()
{
    return d->tree_series;
}

gnomonLStringSeries *gnomonLStringTranslationCommand::inputLString()
{
    return d->lString_series;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonLStringTranslationCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["inputTree"] = this->inputTree();
    inputs["inputLString"] = this->inputLString();
    return inputs;
}

gnomonTreeSeries *gnomonLStringTranslationCommand::outputTree()
{
    return d->output_tree_series;
}

gnomonLStringSeries *gnomonLStringTranslationCommand::outputLString()
{
    return d->output_lString_series;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonLStringTranslationCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
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

void gnomonLStringTranslationCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "inputTree") {
        this->setInputTree(dynamic_cast<gnomonTreeSeries *>(form));
    } else if (name == "inputLString") {
        this->setInputLString(dynamic_cast<gnomonLStringSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonLStringTranslationCommand.cpp ends here
