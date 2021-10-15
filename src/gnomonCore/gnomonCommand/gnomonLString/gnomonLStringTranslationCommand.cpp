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

gnomonLStringTranslationCommand::gnomonLStringTranslationCommand(void) : d(new gnomonLStringTranslationCommandPrivate)
{
    this->factory_name = "lStringTranslation";
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::lStringTranslation::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::lStringTranslation::pluginFactory().create(this->algorithm_name);
    }

}

gnomonLStringTranslationCommand::~gnomonLStringTranslationCommand(void)
{
    delete d;
}

void gnomonLStringTranslationCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::lStringTranslation::pluginFactory().create(algo_name);
}

void gnomonLStringTranslationCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();

    gnomonTreeSeries *tree = ((gnomonAbstractLStringTranslation *) this->action)->outputTree();
    qDebug()<<Q_FUNC_INFO<<tree;
    if ((!tree)||(tree->times().size()==0)) {
        d->output_tree_series = nullptr;
    } else {
        d->output_tree_series = tree;
    }

    gnomonLStringSeries *lString = ((gnomonAbstractLStringTranslation *) this->action)->outputLString();
    qDebug()<<Q_FUNC_INFO<<lString;
    if ((!lString)||(lString->times().size()==0)) {
        d->output_lString_series = nullptr;
    } else {
        d->output_lString_series = lString;
    }
}

void gnomonLStringTranslationCommand::undo(void)
{
    Q_ASSERT(this->action);

    ((gnomonAbstractLStringTranslation *) this->action)->setInputTree(nullptr);
    ((gnomonAbstractLStringTranslation *) this->action)->setInputLString(nullptr);
}

void gnomonLStringTranslationCommand::setInputTree(gnomonTreeSeries* tree_series)
{
    if ((!tree_series)||(tree_series->times().size()==0)) {
        d->tree_series = nullptr;
    } else {
        d->tree_series = tree_series;
        Q_ASSERT(this->action);
        ((gnomonAbstractLStringTranslation *) this->action)->setInputTree(d->tree_series);
    }
}

void gnomonLStringTranslationCommand::setInputLString(gnomonLStringSeries* lString_series)
{
    if ((!lString_series)||(lString_series->times().size()==0)) {
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

QMap<QString, gnomonAbstractDynamicForm *> gnomonLStringTranslationCommand::inputs(void)
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

QMap<QString, gnomonAbstractDynamicForm *> gnomonLStringTranslationCommand::outputs(void)
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["outputTree"] = this->outputTree();
    outputs["outputLString"] = this->outputLString();
    return outputs;
}

dtkCoreParameters gnomonLStringTranslationCommand::parameters(void) const
{
    return this->action->parameters();
}

void gnomonLStringTranslationCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

bool gnomonLStringTranslationCommand::isEmpty(void)
{
    loadPluginGroup("lStringTranslation");
    return gnomonCore::lStringTranslation::pluginFactory().keys().size() == 0;
}

//
// gnomonLStringTranslationCommand.cpp ends here
