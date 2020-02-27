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
};

gnomonLStringTranslationCommand::gnomonLStringTranslationCommand(const QString& key) : d(new gnomonLStringTranslationCommandPrivate)
{
    loadPluginGroup("lStringTranslation");

    this->action = gnomonCore::lStringTranslation::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonLStringTranslationCommand::~gnomonLStringTranslationCommand(void)
{
    delete d;
}

void gnomonLStringTranslationCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->action->run();
}

void gnomonLStringTranslationCommand::undo(void)
{
    Q_ASSERT(this->action);

    ((gnomonAbstractLStringTranslation *) this->action)->setInputTree(nullptr);
    ((gnomonAbstractLStringTranslation *) this->action)->setInputLString(nullptr);
}

void gnomonLStringTranslationCommand::setInputTree(gnomonTreeSeries* tree_series)
{
    d->tree_series = tree_series;
    ((gnomonAbstractLStringTranslation *) this->action)->setInputTree(d->tree_series);
}

gnomonTreeSeries *gnomonLStringTranslationCommand::inputTree()
{
    gnomonTreeSeries *tree = ((gnomonAbstractLStringTranslation *) this->action)->inputTree();
    if ((!tree)||(tree->times().size()==0)) {
        return nullptr;
    } else {
        return tree;
    }
}

void gnomonLStringTranslationCommand::setInputLString(gnomonLStringSeries* lString_series)
{
    d->lString_series = lString_series;
    ((gnomonAbstractLStringTranslation *) this->action)->setInputLString(d->lString_series);
}

gnomonLStringSeries *gnomonLStringTranslationCommand::inputLString()
{
    gnomonLStringSeries *lString = ((gnomonAbstractLStringTranslation *) this->action)->inputLString();
    if ((!lString)||(lString->times().size()==0)) {
        return nullptr;
    } else {
        return lString;
    }
}

gnomonTreeSeries *gnomonLStringTranslationCommand::outputTree()
{
    gnomonTreeSeries *tree = ((gnomonAbstractLStringTranslation *) this->action)->outputTree();
    qDebug()<<Q_FUNC_INFO<<tree;
    if ((!tree)||(tree->times().size()==0)) {
        return nullptr;
    } else {
        return tree;
    }
}

gnomonLStringSeries *gnomonLStringTranslationCommand::outputLString()
{
    gnomonLStringSeries *lString = ((gnomonAbstractLStringTranslation *) this->action)->outputLString();
    qDebug()<<Q_FUNC_INFO<<lString;
    if ((!lString)||(lString->times().size()==0)) {
        return nullptr;
    } else {
        return lString;
    }
}

QMap<QString, gnomonCoreParameter *> gnomonLStringTranslationCommand::parameters(void) const
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
